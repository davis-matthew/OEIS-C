/*
 * A237883  Numbers k such that LR0(k) < LR1(k), where LR0(k) = A087117(k) is
 *          the length of the longest run of zeros in the binary
 *          representation of k, LR1(k) = A038374(k) is the length of the
 *          longest run of ones.
 * https://oeis.org/A237883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that LR0(k) < LR1(k)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the longest runs of zeros and ones in the binary
   representation of x.  The results are returned through *l0 and *l1. */
static inline void lr0_lr1(idx_t x, idx_t *l0, idx_t *l1)
{
    idx_t cur      = x & 1ULL;   /* current run bit (0 or 1)          */
    idx_t run      = 1ULL;       /* length of the current run         */
    idx_t best0    = 0ULL;       /* best zero run found so far        */
    idx_t best1    = 0ULL;       /* best one run found so far         */

    x >>= 1ULL;
    while (x) {
        idx_t bit = x & 1ULL;
        if (bit == cur) {
            run++;                   /* extend current run              */
        } else {
            /* finish the run that just ended */
            if (cur) {                /* it was a run of ones            */
                if (run > best1) best1 = run;
            } else {                  /* it was a run of zeros           */
                if (run > best0) best0 = run;
            }
            cur   = bit;              /* start new run                   */
            run   = 1ULL;
        }
        x >>= 1ULL;
    }

    /* process the final run */
    if (cur) {
        if (run > best1) best1 = run;
    } else {
        if (run > best0) best0 = run;
    }

    *l0 = best0;
    *l1 = best1;
}
/* LR0(k) = longest run of zeros in binary representation of k  */
/* LR1(k) = longest run of ones  in binary representation of k */

/* Returns the n-th term (1‑based) of OEIS sequence A237883. */
seq_t a(idx_t n)
{
    idx_t count = 0ULL;
    idx_t k     = 1ULL;
    idx_t l0, l1;

    while (1) {
        lr0_lr1(k, &l0, &l1);
        if (l0 < l1) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
