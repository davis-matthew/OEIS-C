/*
 * A104217  Period of Perrin (0,2,3,2,5,5,..., A001608) sequence mod n.
 * https://oeis.org/A104217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = period of the Perrin sequence modulo n.
 * The Perrin sequence is defined by
 *   P(0)=3, P(1)=0, P(2)=2 and P(k)=P(k-2)+P(k-3) for k≥3.
 * a(n) is the smallest positive t such that
 *   P(k+t) ≡ P(k) (mod n)  for all integers k.
 */

seq_t a(idx_t n)
{
    /* All calculations are performed modulo n. */
    seq_t mod = (seq_t)n;                 /* n ≥ 1 */

    /* Initial state of the Perrin sequence modulo n. */
    const seq_t init0 = (seq_t)3 % mod;
    const seq_t init1 = (seq_t)0;
    const seq_t init2 = (seq_t)2 % mod;

    /* Current state: positions (k‑3, k‑2, k‑1). */
    seq_t t0 = init0;
    seq_t t1 = init1;
    seq_t t2 = init2;

    seq_t period = 0;

    for (;; ++period)
    {
        /* Next term: P(k) = P(k‑2)+P(k‑3) mod n. */
        seq_t next = t0 + t1;
        if (next >= mod)
            next -= mod;

        /* Shift window. */
        t0 = t1;
        t1 = t2;
        t2 = next;

        /* Check for return to the initial triple. */
        if (t0 == init0 && t1 == init1 && t2 == init2)
            return period + 1;   /* period counts steps, so add 1. */
    }
}
