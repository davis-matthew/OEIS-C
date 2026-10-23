/*
 * A188702  Number of nX5 binary arrays without the pattern 0 0 diagonally or
 *          vertically.
 * https://oeis.org/A188702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------
   Sequence A188702 – number of n×5 binary arrays without
   a 0‑0 pattern on a vertical or diagonal.

   The sequence satisfies a linear recurrence of order 13:
      a(n) =  a(n‑1)  + 49a(n‑2)  + 107a(n‑3)
           –154a(n‑4) – 404a(n‑5) + 250a(n‑6)
           +472a(n‑7) – 278a(n‑8) – 168a(n‑9)
           +131a(n‑10) – 9a(n‑11) – 7a(n‑12) + a(n‑13)
   with initial values
      a(1)..a(13) = {32,144,1665,11556,103484,
                     813309,6814290,55337580,456131965,
                     3733374889,30657827284,251373054600,
                     2062533960693}.
   The function below implements this recurrence iteratively,
   so it works for any n≥1, in any order of calls, and for
   all supported seq_t/idx_t types.
------------------------------------------------------- */

#include <stddef.h>   /* for size_t */

/* external types provided by the build system */
extern seq_t a(idx_t n);   /* declaration for the build */

/* base values for the first 13 terms (index 1..13) */
static const seq_t base_terms[13] = {
    (seq_t)32,          (seq_t)144,          (seq_t)1665,
    (seq_t)11556,       (seq_t)103484,       (seq_t)813309,
    (seq_t)6814290,     (seq_t)55337580,     (seq_t)456131965,
    (seq_t)3733374889,  (seq_t)30657827284,  (seq_t)251373054600,
    (seq_t)2062533960693
};

/*----------------------------------------------------------
   Compute a(n) for any n ≥ 1.  The function works in O(n)
   time and uses only a small fixed amount of RAM.
----------------------------------------------------------*/
seq_t
a(idx_t n)
{
    /* Undefined for n == 0 – return 0 as a harmless value */
    if (n == 0)
        return (seq_t)0;

    /* For n <= 13 return the stored initial value */
    if (n <= 13)
        return base_terms[n - 1];

    /* Circular buffer holding the last 13 terms:
         buffer[0] = a(k‑13), buffer[12] = a(k‑1)          */
    seq_t buffer[13];
    for (size_t i = 0; i < 13; ++i)
        buffer[i] = base_terms[i];

    /* generate terms from a(14) up to a(n) */
    for (idx_t idx = 14; idx <= n; ++idx) {
        /* apply the recurrence using the current buffer */
        seq_t new_val = buffer[12]                                      /* a(k‑1) */
                      + (seq_t)49   * buffer[11]                       /* a(k‑2) */
                      + (seq_t)107  * buffer[10]                       /* a(k‑3) */
                      - (seq_t)154  * buffer[9]                        /* a(k‑4) */
                      - (seq_t)404  * buffer[8]                        /* a(k‑5) */
                      + (seq_t)250  * buffer[7]                        /* a(k‑6) */
                      + (seq_t)472  * buffer[6]                        /* a(k‑7) */
                      - (seq_t)278  * buffer[5]                        /* a(k‑8) */
                      - (seq_t)168  * buffer[4]                        /* a(k‑9) */
                      + (seq_t)131  * buffer[3]                        /* a(k‑10) */
                      - (seq_t)9    * buffer[2]                        /* a(k‑11) */
                      - (seq_t)7    * buffer[1]                        /* a(k‑12) */
                      + buffer[0];                                      /* a(k‑13) */

        /* shift buffer left: discard oldest, insert new */
        for (size_t i = 0; i < 12; ++i)
            buffer[i] = buffer[i + 1];
        buffer[12] = new_val;
    }

    /* after loop, buffer[12] == a(n) */
    return buffer[12];
}
