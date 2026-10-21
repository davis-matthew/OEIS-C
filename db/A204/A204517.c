/*
 * A204517  Square root of floor[A055859(n)/7].
 * https://oeis.org/A204517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n <= 7 then base[n] else 16*a(n-3) - a(n-6)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A204517: sqrt(floor(A055859(n)/7)) with given generating function */
/* The linear recurrence follows from: (x^4 + 3*x^5 + 6*x^6 + x^7)/(1 - 16*x^3 + x^6) */

static const seq_t base_vals[8] = {0, 0, 0, 0, 1, 3, 6, 17};

seq_t a(idx_t n)
{
    /* for safety return 0 for n==0 though not used by the sequence */
    if (n == 0)
        return 0;

    /* for n <= 7 we can directly return the known values */
    if (n <= 7)
        return base_vals[n];

    /* Use a circular buffer holding the last seven terms.  Index i % 7
       holds a(i).  Initialise it with the first seven known terms. */
    seq_t buf[7];
    for (idx_t i = 1; i <= 7; ++i)
        buf[i % 7] = base_vals[i];

    /* Compute terms up to n */
    for (idx_t i = 8; i <= n; ++i) {
        /* a(i) = 16*a(i-3) - a(i-6); indices modulo 7 */
        seq_t term = (seq_t)16 * buf[(i - 3) % 7] - buf[(i - 6) % 7];
        buf[i % 7] = term;
    }

    return buf[n % 7];
}
