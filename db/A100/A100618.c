/*
 * A100618  Initially there are n people in a room. At each step, if there are
 *          currently M people in the room, [M/k^2] of them leave, for k = 2,
 *          3, ... Sequence gives number who are left at the end.
 * https://oeis.org/A100618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 447, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iteratively subtract floor(M/k^2) for k = 2,3,4,… until no more people leave. */

seq_t a(idx_t n)
{
    seq_t M = (seq_t)n;          /* current number of people */
    for (idx_t k = 2; ; ++k)     /* iterate over k */
    {
        seq_t ksqr = (seq_t)k * (seq_t)k; /* k^2 */
        if (ksqr > M)              /* floor(M/k^2) would be 0 */
            break;
        seq_t t = M / ksqr;       /* people leaving at this step */
        M -= t;                   /* update M */
    }
    return M;
}
