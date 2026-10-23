/*
 * A025623  Numbers of form 5^i*8^j, with i, j >= 0.
 * https://oeis.org/A025623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 86, u64 n <= 319,
 *            u128 n <= 1225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025623 – numbers of the form 5^i * 8^j (i, j ≥ 0).  
   The function generates the terms in increasing order by merging the
   two geometric sequences 1,5,25,125,… and 1,8,64,512,…   
   For types that fit in 64 bits an overflow is detected; for larger or
   arbitrary‑precision types no overflow occurs. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1 */

    /* Allocate an array for the requested number of terms */
    seq_t *seq = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!seq)
        return (seq_t)0;          /* unlikely in tests */

    seq[0] = (seq_t)1;
    size_t p5 = 0, p8 = 0;        /* indices for 5‑multiples and 8‑multiples */
    size_t generated = 1;         /* number of terms produced */

    /* Detect if seq_t is large enough that overflow cannot occur */
    int big = sizeof(seq_t) > 8;   /* > 8 bytes → ≥ 64 bits (or arbitrary precision) */

    while (generated < (size_t)n) {
        seq_t cand5 = 0, cand8 = 0;
        int ok5 = 0, ok8 = 0;

        if (big) {
            /* For big or arbitrary‑precision types, overflow is impossible. */
            cand5 = seq[p5] * (seq_t)5;
            cand8 = seq[p8] * (seq_t)8;
            ok5 = ok8 = 1;
        } else {
            seq_t max = (seq_t)-1;                        /* maximum representable value */
            seq_t lim5 = max / (seq_t)5;
            seq_t lim8 = max / (seq_t)8;

            if (seq[p5] <= lim5) {
                cand5 = seq[p5] * (seq_t)5;
                ok5 = 1;
            }
            if (seq[p8] <= lim8) {
                cand8 = seq[p8] * (seq_t)8;
                ok8 = 1;
            }
        }

        if (!ok5 && !ok8)
            break;                     /* cannot generate further terms without overflow */

        seq_t next;
        if (ok5 && ok8)
            next = (cand5 <= cand8) ? cand5 : cand8;
        else if (ok5)
            next = cand5;
        else
            next = cand8;

        seq[generated] = next;
        if (ok5 && cand5 == next)
            ++p5;
        if (ok8 && cand8 == next)
            ++p8;

        ++generated;
    }

    seq_t result = seq[generated - 1];
    free(seq);
    return result;
}
