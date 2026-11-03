/*
 * A143736  Numbers n such that if f[n] = n-th Fibonacci number then
 *          f[n]-n=prime and f[n]+n=prime.
 * https://oeis.org/A143736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = F_k where k is such that F_k - k and F_k + k are both
 *            prime. The function computes the nth such Fibonacci number,
 *            using a memoized search over increasing indices k
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return the n-th Fibonacci number that satisfies the condition
   F_k - k and F_k + k are prime. */
seq_t a(idx_t n) {
    /* Static cache of found values */
    static seq_t *values = NULL;   /* array of seq_t */
    static size_t values_cap = 0;  /* capacity of array */
    static size_t values_len = 0;  /* number of stored values */

    /* Fibonacci state (for incremental computation) */
    static seq_t f_prev = 0;        /* F_{k-1} */
    static seq_t f_curr = 1;        /* F_k   */
    static idx_t next_k = 1;        /* next index to test (k) */

    /* Initialize only once */
    static bool initialized = false;
    if (!initialized) {
        f_prev = 0;
        f_curr = 1;
        next_k = 1;
        initialized = true;
    }

    /* Keep computing until we have at least n values */
    while (values_len < (size_t)n) {
        /* Check the current Fibonacci number */
        seq_t minus = f_curr - (seq_t)next_k;
        seq_t plus  = f_curr + (seq_t)next_k;

        if (is_prime_big(minus) && is_prime_big(plus)) {
            /* Store this Fibonacci number */
            grow_seq(&values, &values_cap, values_len + 1);
            values[values_len++] = f_curr;
        }

        /* Advance Fibonacci sequence to the next index */
        seq_t tmp = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = tmp;
        next_k++;
    }

    return values[n - 1];
}
