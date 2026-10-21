/*
 * A234739  Largest prime divisor of all composite numbers of the form k^2+1
 *          between two consecutive primes of the same form.
 * https://oeis.org/A234739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor among all composite k^2+1 numbers
 *            lying strictly between consecutive primes of the form m^2+1
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 41, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_prime_big, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses the utility routines supplied by the build
    environment:  is_prime_big, largest_prime_divisor, and
    _floor_isqrt_u64.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/largest_prime_divisor.h"
#include "../utils/_floor_isqrt_u64.h"

/* Dynamic array growth helper */
static void
ensure_capacity(idx_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if ((size_t)needed <= *cap) return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    /*  Storage for primes of the form m^2+1 and the sequence values. */
    static seq_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    static uint64_t next_m = 2;       /* start at m = 2 (gives 5) */

    static seq_t *a_values = NULL;
    static size_t a_cnt = 0, a_cap = 0;

    /* Ensure we have at least primes[n] and primes[n+1] */
    idx_t needed_prime_index = n + 1;     /* 0‑based index of last needed prime */
    while (prime_cnt <= (size_t)needed_prime_index) {
        uint64_t cand = (uint64_t)next_m * next_m + 1;
        if (is_prime_big((seq_t)cand)) {
            ensure_capacity(prime_cnt + 1, &prime_cap, (void **)&prime_list,
                            sizeof(seq_t));
            prime_list[prime_cnt++] = (seq_t)cand;
        }
        ++next_m;                         /* try next m */
    }

    /* Compute a values up to n if required */
    while (a_cnt < (size_t)n) {
        idx_t idx = a_cnt;                     /* will compute a(idx+1)   */
        uint64_t lower = (uint64_t)prime_list[idx];
        uint64_t upper = (uint64_t)prime_list[idx + 1];

        uint64_t max_lpd = 0;
        uint64_t start_m = _floor_isqrt_u64(lower - 1) + 1;
        for (uint64_t m = start_m; ; ++m) {
            uint64_t num = m * m + 1;
            if (num >= upper) break;
            if (!is_prime_big((seq_t)num)) {
                uint64_t lpd = largest_prime_divisor(num);
                if (lpd > max_lpd) max_lpd = lpd;
            }
        }

        ensure_capacity(a_cnt + 1, &a_cap, (void **)&a_values, sizeof(seq_t));
        a_values[a_cnt++] = (seq_t)max_lpd;
    }

    return a_values[n - 1];
}
