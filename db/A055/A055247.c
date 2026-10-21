/*
 * A055247  Related to A055246 and A005836. Used for boundaries of open
 *          intervals which have to be erased in the Cantor middle third set
 *          construction.
 * https://oeis.org/A055247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A055246(ceil(n/2)) + (n%2==0) where A055246(k) = 1 +
 *            6*B(k-1) and B(m) is the value of the binary representation of m
 *            read as a base‑3 number
 * Range:     u8 n <= 32, u16 n <= 1024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute k = ceil(n/2) = (n+1)/2 */
    idx_t k = (n + 1) >> 1;          /* k ≥ 1 */

    /* Compute B(k-1): interpret (k-1) as a binary number with digits 0 or 1
       read as a number in base 3. */
    idx_t m = k - 1;                 /* original index for B */
    seq_t power = 1;                 /* current power of 3  (3^pos) */
    seq_t B = 0;                     /* sum of selected powers of 3 */

    while (m > 0) {
        if (m & 1) {
            B += power;
        }
        m >>= 1;
        power *= 3;                  /* next power of 3 */
    }

    /* A055246(k) = 1 + 6*B(k-1) */
    seq_t A = 1 + (seq_t)6 * B;

    /* a(n) is A for odd n, A+1 for even n */
    if (n & 1) {                     /* odd  */
        return A;
    } else {                         /* even */
        return A + 1;
    }
}
