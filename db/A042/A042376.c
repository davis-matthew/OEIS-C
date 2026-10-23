/*
 * A042376  Numerators of continued fraction convergents to sqrt(715).
 * https://oeis.org/A042376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 151292*a(n-10) - a(n-20) with initial values a(0) … a(19)
 *            given below
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 20 terms of the sequence */
    static const seq_t init[20] = {
        (seq_t)26,          /* a(0) */
        (seq_t)27,          /* a(1) */
        (seq_t)80,          /* a(2) */
        (seq_t)107,         /* a(3) */
        (seq_t)615,         /* a(4) */
        (seq_t)3182,        /* a(5) */
        (seq_t)16525,       /* a(6) */
        (seq_t)19707,       /* a(7) */
        (seq_t)55939,       /* a(8) */
        (seq_t)75646,       /* a(9) */
        (seq_t)3989531,     /* a(10) */
        (seq_t)4065177,     /* a(11) */
        (seq_t)12119885,    /* a(12) */
        (seq_t)16185062,    /* a(13) */
        (seq_t)93045195,    /* a(14) */
        (seq_t)481411037,   /* a(15) */
        (seq_t)2500100380,  /* a(16) */
        (seq_t)2981511417,  /* a(17) */
        (seq_t)8463123214,  /* a(18) */
        (seq_t)11444634631  /* a(19) */
    };

    /* Return directly if the requested term is within the initial block. */
    if (n < 20)
        return init[(size_t)n];

    /* Circular buffer holding the last 20 computed terms. */
    seq_t buf[20];
    for (size_t i = 0; i < 20; ++i)
        buf[i] = init[i];

    /* Compute terms from a(20) up to a(n) using the recurrence. */
    for (idx_t i = 20; i <= n; ++i)
    {
        size_t idx  = (size_t)i % 20;
        size_t idx10 = (size_t)(i - 10) % 20;
        size_t idx20 = (size_t)(i - 20) % 20;

        /* Use seq_t arithmetic; the literal 151292 is converted to seq_t. */
        buf[idx] = (seq_t)151292 * buf[idx10] - buf[idx20];
    }

    return buf[(size_t)n % 20];
}
