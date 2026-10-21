/*
 * A073655  Rearrangement of triangular numbers such that sum of two
 *          consecutive terms is a prime.
 * https://oeis.org/A073655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 160, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A073655: Arrange triangular numbers so that sums of consecutive terms
   are prime.  The sequence is built greedily: each new term is the smallest
   unused triangular number that, when added to the preceding term, yields a
   prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

/* cached values ----------------------------------------------------- */
static seq_t *seq = NULL;          /* sequence values  */
static size_t seq_cap = 0;         /* capacity of seq    */
static size_t seq_len = 0;         /* number of values    */

static unsigned char *used = NULL; /* used[k] == 1 iff T_k already used  */
static size_t used_cap = 0;        /* capacity of used[] */

/* ensure that used[0..need-1] is valid */
static void ensure_used_cap(size_t need)
{
    if (used_cap >= need) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    /* initialise new entries to 0 */
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
    used = tmp;
    used_cap = new_cap;
}

/* ensure that seq[0..need-1] is valid */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    while (seq_len < (size_t)n)
    {
        if (seq_len == 0)                 /* initialise first term */
        {
            ensure_seq_cap(1);
            seq[0] = (seq_t)1;
            seq_len = 1;
            ensure_used_cap(2);           /* need index 1 */
            used[1] = 1;
        }
        else
        {
            seq_t prev = seq[seq_len - 1];

            for (idx_t k = 1; ; ++k)      /* search smallest unused T_k */
            {
                ensure_used_cap((size_t)(k + 1));
                if (!used[(size_t)k])
                {
                    seq_t tri = ((seq_t)k * ((seq_t)k + 1)) / 2;
                    if (isPrimeGeneric(prev + tri))
                    {
                        ensure_seq_cap(seq_len + 1);
                        seq[seq_len] = tri;
                        seq_len++;
                        used[(size_t)k] = 1;
                        break;
                    }
                }
            }
        }
    }
    return seq[(size_t)(n - 1)];
}
