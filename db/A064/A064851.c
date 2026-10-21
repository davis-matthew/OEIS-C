/*
 * A064851  Period of continued fraction for sqrt(6)*n.
 * https://oeis.org/A064851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of continued fraction for sqrt(6)*n */
seq_t a(idx_t n)
{
    /* Compute D = 6*n^2 */
    seq_t n_seq = (seq_t)n;
    seq_t D = n_seq * n_seq * (seq_t)6;

    /* Integer floor square root of D via binary search.
       Bound sqrt(D) < 3*n for all positive n. */
    seq_t low = 0;
    seq_t high = (seq_t)3 * n_seq;   /* high > sqrt(D) */
    while (low <= high) {
        seq_t mid = (low + high) / (seq_t)2;
        seq_t mid2 = mid * mid;
        if (mid2 <= D) {
            low = mid + (seq_t)1;
        } else {
            high = mid - (seq_t)1;
        }
    }
    seq_t a0 = high;   /* floor(sqrt(D)) */

    /* Continued‑fraction variables */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_curr = a0;
    seq_t period = 0;

    while (1) {
        /* Next m, d, a according to the standard recurrence. */
        seq_t m_next = d * a_curr - m;
        seq_t num   = D - (m_next * m_next);
        seq_t d_next = num / d;
        seq_t a_next = (a0 + m_next) / d_next;

        period = period + (seq_t)1;

        /* The period ends when a_next equals 2*a0. */
        if (a_next == (a0 * (seq_t)2)) {
            break;
        }

        /* Prepare for next iteration. */
        m = m_next;
        d = d_next;
        a_curr = a_next;
    }

    return period;
}
