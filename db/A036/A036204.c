/*
 * A036204  Log base 2 (n) mod 227.
 * https://oeis.org/A036204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete_log_2(n) mod 227 (base‑2 logarithm modulo 227)
 * Range:     u8 n <= 226, u16 n <= 226, u32 n <= 226, u64 n <= 226,
 *            u128 n <= 226, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Mapping from residue r (1 ≤ r < 227) to exponent e such that
       2^e ≡ r (mod 227).  r = 0 is handled separately. */
    static seq_t exp_of_value[227];
    static uint8_t initialized = 0;

    if (!initialized) {
        idx_t val = 1;                /* 2^0 mod 227 */
        for (idx_t e = 0; e < 226; ++e) {
            exp_of_value[val] = (seq_t)e;
            val = (val * 2) % 227;    /* next power of 2 modulo 227 */
        }
        /* Residue 0 never occurs for 1 ≤ n < 227, but leave entry 0
           as 0 for convenience. */
        exp_of_value[0] = 0;
        initialized = 1;
    }

    idx_t r = n % 227;
    if (r == 0)                    /* n divisible by 227 → log undefined */
        return (seq_t)0;           /* arbitrary placeholder value */
    return exp_of_value[r];
}
