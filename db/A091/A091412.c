/*
 * A091412  Lengths of runs in A090822.
 * https://oeis.org/A091412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal terms in Gijswijt's
 *            sequence A090822 (computed by direct generation of A090822)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9755 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static unsigned int *g_terms = NULL;   /* A090822 terms, 0-based */
static size_t g_len = 0, g_cap = 0;
static unsigned int *g_runs = NULL;    /* completed run lengths */
static size_t r_len = 0, r_cap = 0;
static unsigned int cur_run = 0;

static void A091412_push_term(unsigned int v)
{
    if (g_len == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 1024;
        g_terms = (unsigned int *)realloc(g_terms, g_cap * sizeof(unsigned int));
    }
    if (g_len > 0 && g_terms[g_len - 1] == v) {
        cur_run++;
    } else {
        if (g_len > 0) {
            if (r_len == r_cap) {
                r_cap = r_cap ? r_cap * 2 : 1024;
                g_runs = (unsigned int *)realloc(g_runs, r_cap * sizeof(unsigned int));
            }
            g_runs[r_len++] = cur_run;
        }
        cur_run = 1;
    }
    g_terms[g_len++] = v;
}

static unsigned int A091412_next(void)
{
    size_t len = g_len;
    unsigned int best = 0;
    for (size_t bl = 1; bl <= len; ++bl) {
        size_t start = len - bl;
        unsigned int count = 1;
        while (start >= bl) {
            bool match = true;
            for (size_t j = 0; j < bl; ++j) {
                if (g_terms[start + j] != g_terms[start - bl + j]) { match = false; break; }
            }
            if (!match) break;
            ++count;
            start -= bl;
        }
        if (count > best) best = count;
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (g_len == 0) A091412_push_term(1);
    while (r_len < (size_t)n) {
        unsigned int v = A091412_next();
        A091412_push_term(v);
    }
    return (seq_t)g_runs[(size_t)n - 1];
}
