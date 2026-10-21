/*
 * A076990  a(1) = 1, a(2) = 2; thereafter a(n) = smallest number not
 *          occurring earlier such that the sum of three successive terms is
 *          prime.
 * https://oeis.org/A076990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A076990
 *  a(1) = 1, a(2) = 2; thereafter a(n) = smallest unused integer k such that
 *  a(n-2) + a(n-1) + k is prime.  The sequence is memoised so calls can be
 *  in any order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n) */

/* ************************************************************************
 *  Internal state
 * ************************************************************************/
static seq_t *seq    = NULL;    /* seq[0] holds a(1) */
static size_t seq_cap = 0;
static idx_t  max_computed = 0;

static bool *used     = NULL;   /* used[i] == true if integer i already used   */
static size_t used_cap = 0;

/* ************************************************************************
 *  Resize helpers
 * ************************************************************************/
static void
ensure_seq_capacity(idx_t n)
{
    if ((size_t)n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

static void
ensure_used_capacity(idx_t need)
{
    if ((size_t)need < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 32;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* ************************************************************************
 *  Main sequence function
 * ************************************************************************/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    /* compute missing terms up to n */
    if (n > max_computed)
    {
        ensure_seq_capacity(n);

        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            if (i == 1)
            {
                seq[0] = (seq_t)1;
                ensure_used_capacity(1);
                used[1] = true;
                continue;
            }
            if (i == 2)
            {
                seq[1] = (seq_t)2;
                ensure_used_capacity(2);
                used[2] = true;
                continue;
            }

            /* previous two terms */
            seq_t prev2 = seq[i - 3];   /* a(i-2)  */
            seq_t prev1 = seq[i - 2];   /* a(i-1)  */

            idx_t candidate = 1;
            seq_t cand_val;
            for (;;)
            {
                ensure_used_capacity(candidate);
                if (!used[(size_t)candidate])
                {
                    seq_t sum = prev2 + prev1 + (seq_t)candidate;
                    if (isPrimeGeneric(sum))
                    {
                        cand_val = (seq_t)candidate;
                        break;
                    }
                }
                ++candidate;
            }

            seq[i - 1] = cand_val;
            used[(size_t)candidate] = true;
        }
        max_computed = n;
    }

    return seq[n - 1];
}
