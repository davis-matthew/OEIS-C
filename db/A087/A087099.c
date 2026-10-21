/*
 * A087099  Partial sums of A063914.
 * https://oeis.org/A087099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k=1..n} d(k), where d(k) = k, if k is odd 3k/2 – 1, if
 *            k is even (k≥1)
 * Range:     u8 n <= 20, u16 n <= 323, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for OEIS offset 1, but safe */
        return 0;

    /* Use the closed‑form derived from separating odd and even terms. */
    idx_t m = n >> 1;      /* m = ⌊n/2⌋   */
    seq_t result;

    if ((n & 1U) == 0) {   /* n is even : n = 2*m */
        seq_t mm = (seq_t)m;

        /* m² + (3/2)·m·(m+1) – m  */
        result = mm * mm;         /* m² */
        seq_t t = mm * (mm + 1);  /* m·(m+1)  */
        t /= 2;                   /* (m·(m+1))/2   */
        t *= 3;                   /* (3/2)·m·(m+1) */
        result += t;
        result -= mm;             /* –m  */
    } else {                     /* n is odd : n = 2*m+1 */
        seq_t mp1 = (seq_t)(m + 1);

        /* (m+1)² + (3/2)·m·(m+1) – m  */
        result = mp1 * mp1;        /* (m+1)² */
        seq_t t = (seq_t)m * mp1;  /* m·(m+1) */
        t /= 2;                    /* (m·(m+1))/2  */
        t *= 3;                    /* (3/2)·m·(m+1) */
        result += t;
        result -= (seq_t)m;        /* –m  */
    }

    return result;
}
