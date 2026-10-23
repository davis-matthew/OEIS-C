/*
 * A041983  Denominators of continued fraction convergents to sqrt(514).
 * https://oeis.org/A041983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 9250*a(n-6) - a(n-12) with initial values a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=67, a(4)=137, a(5)=204, a(6)=9113, a(7)=9317,
 *            a(8)=27747, a(9)=619751, a(10)=1267249, a(11)=1887000
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const seq_t init_vals[12] = {
    (seq_t)1ULL,    /* a(0) */
    (seq_t)1ULL,    /* a(1) */
    (seq_t)3ULL,    /* a(2) */
    (seq_t)67ULL,   /* a(3) */
    (seq_t)137ULL,  /* a(4) */
    (seq_t)204ULL,  /* a(5) */
    (seq_t)9113ULL, /* a(6) */
    (seq_t)9317ULL, /* a(7) */
    (seq_t)27747ULL,/* a(8) */
    (seq_t)619751ULL,/* a(9) */
    (seq_t)1267249ULL,/* a(10) */
    (seq_t)1887000ULL /* a(11) */
};

seq_t a(idx_t n)
{
    /* Return directly for the first 12 terms. */
    if (n < 12) {
        return init_vals[(size_t)n];
    }

    /* A ring buffer storing the last 12 computed terms. */
    seq_t ring[12];
    /* Initialise the buffer with the known first 12 values. */
    for (size_t i = 0; i < 12; ++i) {
        ring[i] = init_vals[i];
    }

    /* Compute terms from a(12) up to a(n). */
    for (idx_t i = 12; i <= n; ++i) {
        idx_t idx6  = (i - 6) % 12;
        idx_t idx12 = (i - 12) % 12;
        seq_t value = (seq_t)9250 * ring[idx6] - ring[idx12];
        ring[i % 12] = value;
    }

    /* The desired value is now at position n modulo 12. */
    return ring[n % 12];
}
