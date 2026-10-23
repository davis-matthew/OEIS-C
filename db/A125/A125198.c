/*
 * A125198  Number of magical labelings of the octahedral graph of magic sum
 *          n.
 * https://oeis.org/A125198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            (15*(31+(-1)^n)+1152*n+1216*n^2+720*n^3+250*n^4+48*n^5+4*n^6)/480
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;             /* n^3 */
    seq_t n4 = n3 * n_seq;             /* n^4 */
    seq_t n5 = n4 * n_seq;             /* n^5 */
    seq_t n6 = n5 * n_seq;             /* n^6 */

    /* Polynomial terms */
    seq_t t1 = (seq_t)1152 * n_seq;    /* 1152*n   */
    seq_t t2 = (seq_t)1216 * n2;       /* 1216*n^2 */
    seq_t t3 = (seq_t)720  * n3;       /* 720*n^3  */
    seq_t t4 = (seq_t)250  * n4;       /* 250*n^4  */
    seq_t t5 = (seq_t)48   * n5;       /* 48*n^5   */
    seq_t t6 = (seq_t)4    * n6;       /* 4*n^6    */

    /* (-1)^n part */
    long long diff = ((n & 1) ? -1LL : 1LL);          /* ±1 */
    long long part15 = 15LL * (31 + diff);           /* small integer */
    seq_t part = (seq_t)part15;                      /* cast to seq_t */

    /* Sum all components */
    seq_t sum = part + t1 + t2 + t3 + t4 + t5 + t6;

    /* Final division by 480 */
    return sum / (seq_t)480;
}
