/*
 * A065256  Quintal Queens permutation of N: halve or multiply by 3 (mod 5)
 *          each digit (0->0, 1->3, 2->1, 3->4, 4->2) of the base 5
 *          representation of n.
 * https://oeis.org/A065256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = \sum_{i\ge0} perm[d_i] * 5^i, where n = \sum_{i\ge0} d_i
 *            * 5^i (base‑5 digits d_i)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The permutation 0→0, 1→3, 2→1, 3→4, 4→2 is applied to each
   base‑5 digit and the resulting digits are interpreted again in
   base 5. */
static const unsigned int perm[5] = {0, 3, 1, 4, 2};

seq_t a(idx_t n)
{
    seq_t result = 0;   /* accumulated value in base‑5 form */
    seq_t power  = 1;   /* current 5^i */
    while (n > 0)
    {
        unsigned int d   = n % 5u;          /* current base‑5 digit of n   */
        seq_t mapped      = perm[d];        /* mapped digit  (0,1,2,3,4)   */
        result += mapped * power;           /* add mapped digit * 5^i      */
        power  *= 5;                         /* next power of 5             */
        n     /= 5;                          /* move to next base‑5 digit   */
    }
    return result;
}
