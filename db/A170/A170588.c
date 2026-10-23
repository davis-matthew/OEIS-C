/*
 * A170588  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for 1<=n<=48 a(n)= (n==48?1:2)+∑_{i=0}^{n-1}a(i);
 * for n>48 a(n)=∑_{k=1}^{48}a(n-k); */

/* The function computes a(n) for the Coxeter group with relations (S_i)^2 =
 * (S_i S_j)^48 = I. It uses an O(n) algorithm with a sliding window of size
 * 48 to compute the linear recurrence that follows from the rational
 * generating function. */

seq_t a(idx_t n) {
    /* handle the trivial case */
    if (n == 0) return (seq_t)1;

    /* compute the first 48 terms explicitly */
    seq_t first[49];          /* indices 0..48 */
    first[0] = (seq_t)1;
    seq_t cum = first[0];     /* cumulative sum of terms up to current index */

    if (n <= 48) {
        /* compute all terms up to n */
        idx_t i;
        for (i = 1; i <= n; ++i) {
            seq_t add = (i == 48 ? (seq_t)1 : (seq_t)2);
            seq_t ai = add + cum;
            cum += ai;
            first[i] = ai;
        }
        return first[n];
    }

    /* we have computed 0..48 */
    for (idx_t i = 1; i <= 48; ++i) {
        seq_t add = (i == 48 ? (seq_t)1 : (seq_t)2);
        seq_t ai = add + cum;
        cum += ai;
        first[i] = ai;
    }

    /* set up a circular buffer for the last 48 terms (a(1)..a(48)) */
    seq_t buf[48];
    idx_t head = 0;
    seq_t sum48 = 0;
    for (idx_t j = 0; j < 48; ++j) {
        buf[j] = first[j + 1];  /* a(j+1) */
        sum48 += buf[j];
    }

    seq_t ai;                    /* value of a(current n) */
    for (idx_t i = 49; i <= n; ++i) {
        ai = sum48;                      /* recurrence */
        seq_t old = buf[head];            /* oldest term to drop */
        buf[head] = ai;                   /* insert new term */
        head = (head + 1) % 48;           /* advance head */
        sum48 = sum48 - old + ai;         /* update running sum */
    }

    return ai;
}
