/*
 * A168331  a(n) = (5 + 14*n + 7*(-1)^n)/4.
 * https://oeis.org/A168331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (5 + 14*n + 7*(-1)^n)/4
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a closed‑form expression that avoids any division by 2 issues. */
    seq_t nn = (seq_t)n;                  /* convert index to seq_t */
    seq_t val = (seq_t)7 * nn;            /* 7*n */

    /* Adjust based on parity of n.
       For n even: val = 7*n + 6  ==> (7*n + 6)/2
       For n odd : val = 7*n - 1  ==> (7*n - 1)/2    */
    if (n & 1) {
        val -= 1;                       /* odd n */
    } else {
        val += 6;                       /* even n */
    }

    return val / 2;                      /* final division by 2 */
}
