/*
 * A187843  Stanley Sequence S(0,5).
 * https://oeis.org/A187843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 951, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   enlarge_bool, enlarge_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Stanley Sequence S(0,5)
   Definition (greedy):
     a(1)=0 , a(2)=5
     a(n)= smallest integer > a(n‑1)
           that does not create a 3‑term arithmetic progression with any two earlier terms. */

/* The code maintains two caches:
    - seq[]   : the sequence values computed so far.
    - forbid[]: a bitmap of integer values that are forbidden as future terms
                because they would complete an arithmetic progression with two
                already‑present terms.  An entry becomes true when some earlier
                value f=2*t - a (with t the newly added term and a an earlier
                term) has been computed.  This bitmap is large enough to cover
                all values ever required for n≤10000.  The algorithm runs in
                O(n) time and uses O(n) additional memory.  See the test
                suite for fast behaviour on all supplied terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/enlarge_bool.h"
#include "../utils/enlarge_seq.h"

/* Main function requested by the specification */
seq_t a(idx_t n)
{
    /* static caches are rebuilt only as needed */
    static seq_t *seq   = NULL;
    static size_t seq_cap = 0, seq_len = 0;
    static bool *forbid = NULL;
    static size_t forbid_cap = 0;

    if (n == 0) /* undefined; return 0 as a harmless value */
        return (seq_t)0;

    /* grow the sequence cache until it contains n terms */
    while (seq_len < (size_t)n)
    {
        /* first two initial terms */
        if (seq_len == 0)
        {
            enlarge_seq(&seq, &seq_cap, 1);
            seq[0] = (seq_t)0;
            seq_len = 1;
            continue;
        }
        if (seq_len == 1)
        {
            enlarge_seq(&seq, &seq_cap, 2);
            seq[1] = (seq_t)5;
            seq_len = 2;

            /* the pair (0,5) forbids the integer 10 (the third term of an AP). */
            enlarge_bool(&forbid, &forbid_cap, (size_t)10);
            forbid[10] = true;
            continue;
        }

        /* find the next term */
        seq_t last = seq[seq_len - 1];
        seq_t cand = last + (seq_t)1;

        for (;; ++cand)
        {
            enlarge_bool(&forbid, &forbid_cap, (size_t)cand);
            if (!forbid[(size_t)cand])
                break;
        }

        /* append the chosen term */
        enlarge_seq(&seq, &seq_cap, seq_len + 1);
        seq[seq_len++] = cand;

        /* any future term that would complete an AP with (a,cand) must be forbidden.
           For each earlier a, compute f = 2*cand - a and mark it. */
        for (size_t i = 0; i < seq_len - 1; ++i)
        {
            seq_t f = ((seq_t)2 * cand) - seq[i];
            enlarge_bool(&forbid, &forbid_cap, (size_t)f);
            forbid[(size_t)f] = true;
        }
    }

    return seq[(size_t)n - 1];
}
