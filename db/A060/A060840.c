/*
 * A060840  Number of irreducible representations of symmetric group S_n whose
 *          degree is not divisible by 3.
 * https://oeis.org/A060840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏ g(i) where n = Σ a_i·3^i (base‑3 digits a_i), g(i) = 1
 *            if a_i = 0 = 3^i if a_i = 1 = 3^i·(3^i+3)/2 if a_i = 2
 * Range:     u8 n <= 22, u16 n <= 70, u32 n <= 403, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;     /* running product of g(i) values */
    seq_t pow3   = 1;     /* current power 3^i */
    idx_t x = n;

    /* handle n = 0 as unused (index 0 not part of sequence) */
    while (x > 0) {
        uint_fast8_t digit = x % 3;   /* base‑3 digit a_i */
        if (digit == 1) {
            result *= pow3;           /* g(i) = 3^i */
        } else if (digit == 2) {
            seq_t term = pow3 * (pow3 + 3);
            term /= 2;                /* g(i) = 3^i·(3^i+3)/2 */
            result *= term;
        }
        pow3 *= 3;                    /* move to next higher digit */
        x /= 3;
    }

    return result;
}
