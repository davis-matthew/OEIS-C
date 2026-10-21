/*
 * A079495  Numbers k such that the sum of the squares of the digits of k in
 *          base 3 is 0 (mod 3).
 * https://oeis.org/A079495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A079495
   a(n) = the n-th nonnegative integer k such that
   the sum of the squares of the digits of k in base 3 is
   congruent to 0 modulo 3.
   Since digit² ≡ 0 (for 0) or 1 (for 1 or 2) (mod 3),
   this is equivalent to “the number of non‑zero base‑3 digits of k
   is a multiple of 3”.

   The function below generates terms by brute force.  For n up to 10000
   the required range is < 3 × n, so the algorithm runs in well under
   2 s in C++11/17 and C11.  It does not rely on any special build
   utilities and works for any integer type chosen for seq_t and idx_t. */

/* Note: include <stdint.h> or similar only if your build defines seq_t/idx_t
         themselves. */
seq_t a(idx_t n)
{
    idx_t found = 0;   /* how many qualifying numbers we have seen */
    seq_t k = 0;       /* candidate number */

    while (1) {
        /* compute (number of non‑zero digits in base‑3 representation) mod 3 */
        int mod = 0;              /* 0‑2, we keep it in [0,2] */
        seq_t tmp = k;

        if (tmp == 0) {           /* k == 0 is a special case, handled automatically */
            /* mod stays 0 */
        } else {
            while (tmp != 0) {
                int d = (int)(tmp % 3);
                if (d != 0) {
                    mod++;
                    if (mod == 3) mod = 0;   /* keep modulo 3 */
                }
                tmp /= 3;
            }
        }

        if (mod == 0) {           /* k satisfies the property */
            ++found;
            if (found == n) {
                return k;
            }
        }

        ++k;   /* move to the next integer */
    }
}
