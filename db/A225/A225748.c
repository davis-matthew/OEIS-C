/*
 * A225748  Numbers n for which the sum of the numbers in the Collatz (3x+1)
 *          iteration of n is prime.
 * https://oeis.org/A225748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth natural number n ≥ 2 such that the sum of the *
 *            Collatz (3x+1) iteration starting at n is a prime. a(1)=2,
 *            a(2)=4, …
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *sumCache = NULL;      /* cache of Collatz sums, indexed by n */
static size_t sumCacheSize = 0;     /* current allocated size (in entries) */
static seq_t *found = NULL;         /* sequence of n that satisfy the
                                      property */
static size_t foundSize = 0;        /* number of values currently stored */
static size_t foundCap = 0;         /* allocated capacity of found[] */
static idx_t nextCandidate = 2;     /* next n to test */

/* -----------------------------------------------------------------------
   Compute the sum of the Collatz sequence starting at idx_t m.
   The result is cached for future calls.
   ----------------------------------------------------------------------- */
static seq_t get_collatz_sum(idx_t m)
{
    if (m == 1)                /* base case: 1 → 1 */
        return 1;

    /* Return cached value if available */
    if (m < sumCacheSize && sumCache[m] != 0)
        return sumCache[m];

    /* Grow the cache if m is outside its current bounds */
    if ((size_t)m >= sumCacheSize) {
        size_t newSize = (size_t)m + 1;
        seq_t *tmp = mem_resize(seq_t, sumCache, newSize);
        if (!tmp) abort();                /* allocation failure */
        /* initialise the new entries to 0 */
        for (size_t i = sumCacheSize; i < newSize; ++i)
            tmp[i] = 0;
        sumCache = tmp;
        sumCacheSize = newSize;
    }

    /* Compute next value in the Collatz sequence */
    idx_t next = (m & 1) ? (3 * m + 1) : (m / 2);

    /* Recursive sum */
    seq_t sum = (seq_t)m + get_collatz_sum(next);

    /* Cache and return */
    sumCache[m] = sum;
    return sum;
}

/* -----------------------------------------------------------------------
   Ensure that the found[] array can hold at least 'need' elements.
   ----------------------------------------------------------------------- */
static void ensure_found_capacity(size_t need)
{
    if (need <= foundCap)
        return;
    size_t newCap = (foundCap == 0) ? 16 : foundCap << 1;
    while (newCap < need)
        newCap <<= 1;
    seq_t *tmp = mem_resize(seq_t, found, newCap);
    if (!tmp) abort();                    /* allocation failure */
    found = tmp;
    foundCap = newCap;
}

/* -----------------------------------------------------------------------
   Public function: a(n) returns the nth term of sequence A225748.
   ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined; return 0 as safety */
        return 0;

    /* Generate terms until we have at least n values */
    while (foundSize < (size_t)n) {
        seq_t sum = get_collatz_sum(nextCandidate);
        if (is_prime_big(sum)) {
            ensure_found_capacity(foundSize + 1);
            found[foundSize++] = nextCandidate;
        }
        ++nextCandidate;
    }
    return found[n - 1];            /* 1‑based indexing */
}
