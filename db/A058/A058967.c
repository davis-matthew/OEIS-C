/*
 * A058967  If n < 8 then A058966(n), else n*2^(n - 3) - 2*n - 50.
 * https://oeis.org/A058967
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*2^(n-3) - 2*n - 50 for n ≥ 8, a(n) = A058966(n) for 3 ≤
 *            n ≤ 7. The constants for A058966 are hard‑coded because the rule
 *            is limited to those small indices
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices directly (A058966 values). */
    if (n < 8) {
        switch ((int)n) {
            case 3: return (seq_t)1;   /* A058966(3) */
            case 4: return (seq_t)2;   /* A058966(4) */
            case 5: return (seq_t)10;  /* A058966(5) */
            case 6: return (seq_t)30;  /* A058966(6) */
            case 7: return (seq_t)78;  /* A058966(7) */
            default: return (seq_t)0;   /* never reached for valid n≥3 */
        }
    }

    /* For n ≥ 8 compute n * 2^(n-3) - 2*n - 50.
       2^(n-3) is built by repeated multiplication by 2. */
    seq_t pow2 = (seq_t)1;          /* 2^0 */
    for (idx_t i = 3; i < n; ++i) { /* multiply by 2 (n-3) times */
        pow2 *= (seq_t)2;
    }

    seq_t n_val = (seq_t)n;
    seq_t result = n_val * pow2;   /* n * 2^(n-3) */
    result -= n_val;               /* subtract n once */
    result -= n_val;               /* subtract n again (total 2*n) */
    result -= (seq_t)50;           /* subtract 50 */
    return result;
}
