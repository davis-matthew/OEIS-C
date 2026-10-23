/*
 * A162935  Highly composite numbers (A002182) with property that the next
 *          highly composite number is more than 3/2 times greater.
 * https://oeis.org/A162935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Subsequence of highly composite numbers (A002182) where the next
   highly composite number is more than 3/2 times greater.  The sequence
   is computed once by a divisor–sieve up to a fixed limit.  It is
   memoised for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state */
static bool initialized = false;
static idx_t *records      = NULL; /* all highly composite numbers found   */
static size_t rec_cnt      = 0;
static size_t rec_cap      = 0;
static idx_t *subseq       = NULL; /* subsequence that satisfies the ratio */
static size_t sub_cnt      = 0;
static size_t sub_cap      = 0;

#define LIMIT 1000000U   /* sufficient to cover every relevant highly
                           composite number for this sequence        */

static void init_sequence(void)
{
    if (initialized)
        return;

    /* 1. Compute divisor count of every integer up to LIMIT with a sieve. */
    uint32_t *divcnt = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!divcnt)
        abort();                     /* out of memory */

    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i)
            divcnt[j]++;              /* divisor count increment          */
    }

    /* 2. Extract highly composite numbers (record divisor counts). */
    uint32_t max_dcnt = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (divcnt[i] > max_dcnt) {
            max_dcnt = divcnt[i];
            if (rec_cnt >= rec_cap) {
                rec_cap = rec_cap ? rec_cap * 2 : 32;
                records = (idx_t *)realloc(records,
                                            rec_cap * sizeof(idx_t));
                if (!records)
                    abort();
            }
            records[rec_cnt++] = i;
        }
    }

    /* 3. Build the desired subsequence: keep a HC if next HC > 3/2 * current. */
    sub_cap = 32;
    subseq = (idx_t *)malloc(sub_cap * sizeof(idx_t));
    if (!subseq)
        abort();

    for (size_t k = 0; k + 1 < rec_cnt; ++k) {
        idx_t cur = records[k];
        idx_t nxt = records[k + 1];
        /* 2*nxt > 3*cur → nxt * 2 > cur * 3 */
        if ((seq_t)nxt * 2 > (seq_t)cur * 3) {
            if (sub_cnt >= sub_cap) {
                sub_cap *= 2;
                subseq = (idx_t *)realloc(subseq,
                                          sub_cap * sizeof(idx_t));
                if (!subseq)
                    abort();
            }
            subseq[sub_cnt++] = cur;
        }
    }

    free(divcnt);
    initialized = true;
}

/* Public function – required entry point */
seq_t a(idx_t n)
{
    init_sequence();

    if (n < 1 || (idx_t)n > sub_cnt)
        return (seq_t)0;               /* undefined/empty for out‑of‑range n */

    return (seq_t)subseq[(size_t)(n - 1)];
}
