/*
 * A214036  Numbers k such that floor(sqrt(1)) + floor(sqrt(2)) +
 *          floor(sqrt(3)) + ... + floor(sqrt(k)) is prime.
 * https://oeis.org/A214036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = k-th integer k such that * S(k) = Σ_{i=1}^k ⌊√i⌋ is
 *            prime. * The function below generates terms on demand, caching
 *            them in a * static array so that calls in any order are
 *            efficient
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: full, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, NULL, size_t */
#include <stdint.h>     /* uint64_t */
#include <stdbool.h>    /* bool */
#include "../utils/isqrt.h"
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

static idx_t   last_k   = 0;      /* last k processed                               */
static seq_t   sum      = 0;      /* current value of Σ_{i=1}^{last_k} ⌊√i⌋          */
static idx_t  *values   = NULL;   /* array of k values where the sum is prime         */
static size_t  val_len  = 0;      /* number of values stored                          */
static size_t  val_cap  = 0;      /* capacity of the values array                    */

/* grow the values array if needed */
static void
grow_values(void)
{
    if (val_len + 1 > val_cap) {
        size_t new_cap = (val_cap == 0) ? 16 : val_cap << 1;
        idx_t *p = (idx_t *)realloc(values, new_cap * sizeof(idx_t));
        if (p == NULL) {
            /* allocation failure – terminate the program */
            abort();
        }
        values = p;
        val_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Transform the 1‑based index to a 0‑based index for the array */
    idx_t target_index = n - 1;
    if (n == 0) {
        return 0;          /* undefined, but safe fallback */
    }

    /* Generate terms until we have at least n of them */
    while (val_len <= target_index) {
        /* Advance to the next k */
        last_k++;
        /* Add ⌊√last_k⌋ to the running sum */
        uint64_t inc = (uint64_t)isqrt(last_k);
        sum += (seq_t)inc;

        /* Check if the current sum is a prime */
        if (is_prime_big(sum)) {
            grow_values();
            values[val_len++] = last_k;
        }
    }

    return values[target_index];
}
