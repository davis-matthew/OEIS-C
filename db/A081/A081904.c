/*
 * A081904  Second binomial transform of binomial(n+6, 6).
 * https://oeis.org/A081904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081904: Second binomial transform of binomial(n+6,6)
/* Recurrence derived from the generating function (1-2x)^6/(1-3x)^7: */
/*   a(n) = 21*a(n-1) - 189*a(n-2) + 945*a(n-3) - 2835*a(n-4) + 5103*a(n-5) - 5103*a(n-6) + 2187*a(n-7) */
/*   for n >= 7, with a(0)=1, a(1)=9, a(2)=60, a(3)=344, a(4)=1794, a(5)=8754, a(6)=40636. */

#include <stddef.h>

/* Multiply a seq_t by a small unsigned integer using a wider intermediate type if needed. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ <= 16)
    /* If seq_t is at most 64 bits, compute in 128 bits to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        return (seq_t)((unsigned __int128)(uint64_t)x * m);
    }
#else
    (void)m; /* silence unused warning if no 128-bit available */
#endif
    return x * m;
}

/* First seven terms of the sequence.  These are used as initial conditions for the recurrence. */
static const seq_t init_vals[7] = {
    1, 9, 60, 344, 1794, 8754, 40636
};

/* Compute a(n).  The function is re‑entrant and works in both C11 and C++17. */
seq_t a(idx_t n)
{
    if (n < 7) {
        return init_vals[n];
    }

    /* Circular buffer holding the last seven computed values. */
    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i) {
        buf[i] = init_vals[i];
    }

    /* Iterate from 7 up to n, each step using the recurrence. */
    for (idx_t k = 7; k <= n; ++k) {
        seq_t term;
        /* Positive contributions */
        term = mul_small(buf[(k - 1) % 7], 21);
        term = term + mul_small(buf[(k - 3) % 7], 945);
        term = term + mul_small(buf[(k - 5) % 7], 5103);
        term = term + mul_small(buf[(k - 7) % 7], 2187);

        /* Negative contributions */
        term = term - mul_small(buf[(k - 2) % 7], 189);
        term = term - mul_small(buf[(k - 4) % 7], 2835);
        term = term - mul_small(buf[(k - 6) % 7], 5103);

        /* Store the new value in the circular buffer. */
        buf[k % 7] = term;
    }

    return buf[n % 7];
}
