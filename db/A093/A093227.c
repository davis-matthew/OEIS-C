/*
 * A093227  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 27.
 * https://oeis.org/A093227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093227 – largest decimal number of length 2*n‑1 whose all
   n‑digit substrings are distinct and divisible by 27.
   Pattern:
   n = 1          : 0
   n = 2          : 81
   n ≥ 3
      let r = n mod 3
      r = 1 : set digit at position n‑1 to 0
      r = 2 : set digits at positions n‑2, n‑1, 2n‑2 to 0
      r = 0 : set digits at positions n‑2 and 2n‑2 to 7, 
              set digit at position n‑1 to 2 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)81;

    const idx_t L = (n << 1) - 1;            /* 2*n - 1 digits */
    const idx_t r = n % 3;                   /* mod 3 */

    seq_t result = (seq_t)0;

    /* pre‑compute positions that may be special */
    const idx_t pos_n2 = n - 2;              /* for n ≥ 3 */
    const idx_t pos_n1 = n - 1;
    const idx_t pos_2n2 = (n << 1) - 2;        /* 2*n - 2 */

    for (idx_t i = 0; i < L; ++i) {
        unsigned d = 9;                      /* default digit */

        if (r == 1) {
            if (i == pos_n1) d = 0;
        } else if (r == 2) {
            if (i == pos_n2 || i == pos_n1 || i == pos_2n2) d = 0;
        } else { /* r == 0 */
            if (i == pos_n2) d = 7;
            else if (i == pos_n1) d = 2;
            else if (i == pos_2n2) d = 7;
        }

        result = result * (seq_t)10 + (seq_t)d;
    }

    return result;
}
