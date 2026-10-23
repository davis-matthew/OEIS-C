/*
 * A104512  a(n) is the minimum number that is the first of k > 1 consecutive
 *          integers whose sum equals n, or 0 if impossible.
 * https://oeis.org/A104512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the smallest a>0 such that n = k*a + k*(k‑1)/2 for some
 *            integer k>1; a(n) = 0 if no such representation exists
 * Range:     u8 n <= 519, u16 n <= 9362, u32 n <= 9362, u64 n <= 9362,
 *            u128 n <= 9362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* cast the index to the working integer type */
    seq_t N = (seq_t)n;
    seq_t best = (seq_t)0;   /* 0 means "not found yet" */

    /* iterate over possible run lengths k (k>1) */
    for (idx_t k = 2; ; ++k)
    {
        seq_t k_seq = (seq_t)k;          /* k as seq_t */
        seq_t offset = k_seq * (k_seq - (seq_t)1) / (seq_t)2; /* k*(k-1)/2 */

        /* if offset >= N then a would be <= 0 for this and all larger k */
        if (offset >= N)
            break;

        seq_t diff = N - offset;
        if (diff % k_seq != (seq_t)0)        /* n not divisible into k blocks */
            continue;

        seq_t a_start = diff / k_seq;       /* candidate starting integer */
        if (a_start < (seq_t)1)             /* need a>=1 */
            continue;

        if (best == (seq_t)0 || a_start < best)
            best = a_start;
    }

    return best;  /* 0 if none found */
}
