/*
 * A068765  Generalized Catalan numbers 3*x*A(x)^2 -A(x) +1 -2*x = 0.
 * https://oeis.org/A068765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+1)*a(n) = 24*(2-n)*a(n-2) + 6*(2*n-1)*a(n-1), a(0) = 1, a(1)
 *            = 1 (This is OEIS A068765 – Generalized Catalan numbers)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t and __uint128_t if available */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Working variables for the two previous terms */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) – initially a(0) */
    seq_t a_nm1 = (seq_t)1;   /* a(n-1) – initially a(1) */
    seq_t a_n   = (seq_t)1;   /* current a(n) */

    /* Helper to decide whether we have a 64‑bit build that can use
       an intermediate 128‑bit type. */
#if defined(__SIZEOF_INT128__)
#   define USE_INT128   (sizeof(seq_t) <= 8)
#else
#   define USE_INT128   0
#endif

    for (idx_t k = 2; k <= n; ++k) {
        /* For the recurrence we need: 
           a(k) = (6*(2k-1)*a(k-1) - 24*(k-2)*a(k-2)) / (k+1)   */
        if (USE_INT128) {
            /* 64‑bit build – use __uint128_t to avoid overflow */
            __uint128_t t1 = (__uint128_t)a_nm1 * (6ULL * (2ULL * k - 1ULL));
            __uint128_t t2 = (__uint128_t)a_nm2 * (24ULL * (k - 2ULL));
            /* t1 >= t2 for all k >= 2, so subtraction is safe */
            __uint128_t prod = (t1 - t2) / (k + 1ULL);
            a_n = (seq_t)prod;          /* fits into seq_t because the result does */
        } else {
            /* seq_t is at least 128 bits or arbitrary‑precision – use
               seq_t arithmetic directly, which handles large intermediates. */
            seq_t t1 = a_nm1 * (seq_t)(6ULL * (2ULL * k - 1ULL));
            seq_t t2 = a_nm2 * (seq_t)(24ULL * (k - 2ULL));
            a_n = (t1 - t2) / (seq_t)(k + 1ULL);
        }

        /* Prepare for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
