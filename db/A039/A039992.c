/*
 * A039992  Number of distinct primes embedded in prime p(n).
 * https://oeis.org/A039992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct primes that can be formed from * some
 *            or all digits of the n-th prime
 * Range:     u8 n <= 27, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: partial, pass, 101 terms through n = 101 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

#define MAX_NUM 1000000   /* enough for primes up to 104729 (6 digits) */

static uint8_t  seen[MAX_NUM];      /* 0/1 flag for numbers seen in this call */
static uint32_t seen_list[MAX_NUM]; /* list of numbers seen, to reset later */
static size_t   seen_list_cnt;

/* Recursive helper: cur = current number built, freq = remaining digit counts,
 * started = whether cur is non-zero (to avoid leading zeros),
 * count = accumulator for distinct primes found.
 */
static void dfs(seq_t cur, int *freq, bool started,
                seq_t *count, size_t *list_cnt)
{
    for (int d = 0; d < 10; ++d) {
        if (!freq[d]) continue;
        if (!started && d == 0) continue; /* avoid leading zeros */

        freq[d]--;                                 /* use digit d */
        seq_t new_val = cur * 10 + d;              /* new number */

        if (new_val < MAX_NUM) {
            size_t idx = (size_t)new_val;
            if (!seen[idx]) {
                seen[idx] = 1;
                seen_list[(*list_cnt)++] = (uint32_t)new_val;
                if (isPrimeGeneric(new_val)) (*count)++;
            }
        }

        dfs(new_val, freq, true, count, list_cnt);
        freq[d]++;                                 /* backtrack */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                           /* undefined for n<1 */

    uint32_t p = get_prime(n);                     /* n-th prime */
    int digit_cnt[10] = {0};

    /* Count digits of p */
    uint32_t tmp = p;
    do {
        digit_cnt[tmp % 10]++;
        tmp /= 10;
    } while (tmp);

    seq_t primes_found = 0;
    seen_list_cnt = 0;

    dfs(0, digit_cnt, false, &primes_found, &seen_list_cnt);

    /* Reset seen flags for numbers generated this call */
    for (size_t i = 0; i < seen_list_cnt; ++i) {
        seen[seen_list[i]] = 0;
    }

    return primes_found;
}
