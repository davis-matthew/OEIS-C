/*
 * A080705  Numbers k such that A080704(k+1) - A080704(k) = 1.
 * https://oeis.org/A080705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A080704(k+1)-A080704(k)=1; computed by
 *            generating A080704 (A080704(1)=2; for m>1, if m is a term then
 *            A080704(m)=smallest even > A080704(m-1), else A080704(m-1)+3)
 *            and recording k where consecutive terms differ by 1
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A080705_terms = NULL;
static size_t A080705_tcap = 0;
static size_t A080705_tlen = 0;
static size_t A080705_ptr = 0;
static seq_t *A080705_found = NULL;
static size_t A080705_fcap = 0;
static size_t A080705_flen = 0;

seq_t a(idx_t n)
{
    while (A080705_flen < (size_t)n) {
        size_t i = A080705_tlen + 1;
        if (A080705_tcap < i) {
            size_t nc = A080705_tcap ? A080705_tcap * 2 : 1024;
            seq_t *t = mem_resize(seq_t, A080705_terms, nc);
            if (!t) abort();
            A080705_terms = t;
            A080705_tcap = nc;
        }
        seq_t val;
        seq_t prev = 0;
        if (i == 1) {
            val = 2;
        } else {
            prev = A080705_terms[A080705_tlen - 1];
            seq_t si = (seq_t)i;
            while (A080705_ptr < A080705_tlen && A080705_terms[A080705_ptr] < si)
                A080705_ptr++;
            bool in = (A080705_ptr < A080705_tlen && A080705_terms[A080705_ptr] == si);
            if (in) {
                if ((prev & 1) == 0) val = prev + 2;
                else val = prev + 1;
            } else {
                val = prev + 3;
            }
        }
        A080705_terms[A080705_tlen++] = val;
        if (i >= 2 && val - prev == 1) {
            if (A080705_fcap <= A080705_flen) {
                size_t nc = A080705_fcap ? A080705_fcap * 2 : 256;
                seq_t *f = mem_resize(seq_t, A080705_found, nc);
                if (!f) abort();
                A080705_found = f;
                A080705_fcap = nc;
            }
            A080705_found[A080705_flen++] = (seq_t)(i - 1);
        }
    }
    return A080705_found[(size_t)n - 1];
}
