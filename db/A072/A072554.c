/*
 * A072554  E24 range of preferred resistor values in electronic engineering.
 * https://oeis.org/A072554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = base[(n-1)%24] * 10^((n-1)/24) where base =
 *            {10,11,12,13,15,16,18,20,22,24,27,30,33,36,39,43,47,51,56,62,68,75,82,91}
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for the E24 series (scaled by 10^0) */
    static const seq_t base[24] = {
        (seq_t)10,  (seq_t)11,  (seq_t)12,  (seq_t)13,  (seq_t)15,
        (seq_t)16,  (seq_t)18,  (seq_t)20,  (seq_t)22,  (seq_t)24,
        (seq_t)27,  (seq_t)30,  (seq_t)33,  (seq_t)36,  (seq_t)39,
        (seq_t)43,  (seq_t)47,  (seq_t)51,  (seq_t)56,  (seq_t)62,
        (seq_t)68,  (seq_t)75,  (seq_t)82,  (seq_t)91
    };

    /* Convert n to zero‑based index */
    idx_t zero_based = n - (idx_t)1;

    /* Find the base index (0‑23) and the power of ten exponent */
    idx_t base_idx = zero_based % (idx_t)24;
    idx_t exp = zero_based / (idx_t)24;

    /* Compute 10^exp in the seq_t type */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < exp; ++i)
        pow10 *= (seq_t)10;

    /* Return the result */
    return base[base_idx] * pow10;
}
