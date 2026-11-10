/*
 * A073547  Numbers k such that antid(k) = antid(k+1), where antid(k) =
 *          A066272(k).
 * https://oeis.org/A073547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k >= 1 with A066272(k) = A066272(k+1), where
 *            A066272(k) = A000005(2k-1)+A000005(2k+1)+A001227(k)-5 for k>=2,
 *            A066272(1)=0; found by a linear scan, cached
 * Range:     u8 n <= 62, u16 n <= 7410, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8599 (2026-11-07)
 * Helpers:   num_divisors_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/num_divisors_idx.h"

static seq_t A066272_a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }
    idx_t two_n_minus1 = n + n - 1;
    idx_t two_n_plus1  = n + n + 1;
    seq_t d1 = num_divisors_idx(two_n_minus1);
    seq_t d2 = num_divisors_idx(two_n_plus1);
    idx_t n_odd = n;
    while ((n_odd & 1ULL) == 0) {
        n_odd >>= 1;
    }
    seq_t odd_cnt = num_divisors_idx(n_odd);
    seq_t result = d1 + d2 + odd_cnt;
    result -= 5;
    return result;
}

static idx_t *A073547_terms = NULL;
static size_t A073547_cnt = 0;
static size_t A073547_cap = 0;
static idx_t A073547_next = 1;      /* next k to test */
static seq_t A073547_cur = 0;       /* A066272(A073547_next) when valid */
static int A073547_have = 0;

seq_t a(idx_t n)
{
    while (A073547_cnt < (size_t)n) {
        if (!A073547_have) {
            A073547_cur = A066272_a(A073547_next);
            A073547_have = 1;
        }
        seq_t nxt = A066272_a(A073547_next + 1);
        if (nxt == A073547_cur) {
            if (A073547_cnt == A073547_cap) {
                size_t nc = A073547_cap ? A073547_cap * 2 : 1024;
                A073547_terms = (idx_t *)realloc(A073547_terms, nc * sizeof(idx_t));
                A073547_cap = nc;
            }
            A073547_terms[A073547_cnt++] = A073547_next;
        }
        A073547_cur = nxt;
        A073547_next++;
    }
    return (seq_t)A073547_terms[(size_t)n - 1];
}
