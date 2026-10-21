/*
 * A066664  Composite numbers n whose divisors less than or equal to sqrt(n)
 *          are consecutive, from 1 up to some number k.
 * https://oeis.org/A066664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066664: composite numbers n whose divisors ≤ √n are consecutive from 1 up to some k.
   The terms are all numbers of the form 2 p with p prime, together with the fixed
   exceptions 8, 12, 18, 24, and 60. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *seq = NULL;          /* cached sequence values          */
static size_t seq_cap = 0;         /* current allocation capacity     */
static size_t seq_len = 0;         /* number of values stored         */
static idx_t cur_val = 3;          /* last integer examined           */

static const idx_t EXCEPTIONS[] = {8, 12, 18, 24, 60};
static const size_t EXC_COUNT = 5;

/* Ensure at least n terms have been computed and cached. */
static void ensure_terms(idx_t n)
{
    /* Allocate initial buffer if needed. */
    if (!seq) {
        seq_cap = 16;
        seq = seq_alloc(seq_cap);
    }

    /* Generate terms until we have n of them. */
    while (seq_len < (size_t)n) {
        cur_val += 1;
        idx_t m = cur_val;

        /* Check for the form 2·p with p prime. */
        if ((m & 1) == 0) {
            idx_t p = m >> 1;
            if (is_prime(p)) {
                /* Grow the buffer if full. */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap * 2;
                    seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = (seq_t)m;
                continue;
            }
        }

        /* Check if m is one of the fixed exceptions. */
        for (size_t i = 0; i < EXC_COUNT; ++i) {
            if (m == EXCEPTIONS[i]) {
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap * 2;
                    seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = (seq_t)m;
                break;
            }
        }
        /* Non‑matching numbers are skipped. */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; return 0 for out‑of‑range input */
    ensure_terms(n);
    return seq[n - 1];
}
