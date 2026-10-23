/*
 * A036163  Log base 3 (n) mod 17.
 * https://oeis.org/A036163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = exponent e such that 3^e ≡ n (mod 17);  
   if n ≡ 0 (mod 17) return 0. */
seq_t a(idx_t n)
{
    seq_t r = (seq_t)(n % 17);
    if (r == 0) return 0;
    seq_t val = 1;
    for (seq_t e = 0; e < 16; ++e)
    {
        if (val == r) return e;
        val = (val * 3) % 17;
    }
    return 0; /* should never reach here */
}
