/*
 * A171401  Numbers m such that exactly one editing step (insert or
 *          substitute) is necessary to transform the binary representation of
 *          m into the least prime not less than m.
 * https://oeis.org/A171401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 700, u32 n <= 700, u64 n <= 700,
 *            u128 n <= 700, big any n
 * Helpers:   bitlen_seq_t, is_prime_big, is_substitution_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A171401 by brute‑force search.
   a(n) is the n‑th integer m such that the binary Levenshtein
   distance between m and the smallest prime >= m equals 1
   (exactly one insert or substitute).  The algorithm
   keeps a cache of all found values; it grows the cache
   on demand. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_substitution_ok.h"

/* --------------------------------------------------------------------- */
/* helper: a single substitution changes one bit of m into p           */

/* --------------------------------------------------------------------- */
/* helper: p is obtained by inserting one bit into the binary of m      */
/* --------------------------------------------------------------------- */
static bool is_insertion_ok(seq_t m, seq_t p)
{
    idx_t len_m = (m == (seq_t)0) ? (idx_t)1 : bitlen_seq_t(m);
    idx_t len_p = bitlen_seq_t(p);
    if (len_p != len_m + 1) return false;

    for (idx_t pos = 0; pos < len_p; ++pos) {
        seq_t low_mask = (pos == 0) ? (seq_t)0
                                    : (((seq_t)1 << pos) - (seq_t)1);
        seq_t high = p >> (pos + 1);
        seq_t low  = p & low_mask;
        seq_t cand = (high << pos) | low;
        if (cand == m) return true;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* helper: smallest prime >= m                                         */
static seq_t next_prime(seq_t m)
{
    if (m <= (seq_t)2) return (seq_t)2;
    seq_t cand = m;
    if ((cand & (seq_t)1) == (seq_t)0) cand += (seq_t)1;  /* make odd */
    for (;;) {
        if (is_prime_big(cand)) return cand;
        cand += (seq_t)2;
    }
}

/* --------------------------------------------------------------------- */
/* helper: does m satisfy the defining condition?                       */
static bool m_is_valid(seq_t m)
{
    seq_t p = next_prime(m);
    idx_t len_m = (m == (seq_t)0) ? (idx_t)1 : bitlen_seq_t(m);
    idx_t len_p = bitlen_seq_t(p);
    return (len_p == len_m) ? is_substitution_ok(m, p)
                            : is_insertion_ok(m, p);
}

/* --------------------------------------------------------------------- */
/* main sequence function                                                */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out of range, defined to 0 */

    static seq_t *seq = NULL;   /* cache of found values           */
    static size_t  cap  = 0;    /* allocated capacity              */
    static size_t  cnt  = 0;    /* number of elements stored       */
    static idx_t   next = 0;    /* next integer to test            */

    while (cnt < n) {
        for (;;) {
            seq_t m = (seq_t)next;
            ++next;
            if (m_is_valid(m)) {
                if (cnt == cap) {                     /* grow cache    */
                    size_t newcap = (cap) ? cap * 2 : 1;
                    while (newcap <= cnt) newcap <<= 1;
                    seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
                    if (!tmp) abort();                /* allocation fail */
                    if (seq) {
                        memcpy(tmp, seq, cnt * sizeof(seq_t));
                        free(seq);
                    }
                    seq = tmp;
                    cap = newcap;
                }
                seq[cnt++] = m;
                break;
            }
        }
    }
    return seq[n - 1];
}
