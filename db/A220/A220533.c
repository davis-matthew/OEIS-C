/*
 * A220533  a(n) is minimal number such that the set of all composite numbers
 *          <= a(n) contains complete residue system modulo n.
 * https://oeis.org/A220533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A220533
    a(n) = minimal m such that the composite numbers ≤ m contain all
    residue classes modulo n.
    The implementation builds a dynamic sieve of primes and maintains
    an array of composite numbers.  For each n the residues reached
    by scanning the composite list are tracked until the complete
    residue system modulo n is seen; the last composite that fills the
    missing residue is returned.  If the sieve limit is not yet large
    enough the sieve is expanded and the search repeated. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* global sieve state ---------------------------------------------------- */
static size_t global_limit = 0;          /* largest number fully sieved     */
static uint8_t *prime_flag = NULL;       /* 1 = prime, 0 = composite       */
static idx_t *composite_list = NULL;     /* array of composite numbers      */
static size_t comp_cap = 0;              /* allocated capacity              */
static size_t comp_cnt = 0;              /* number of composites stored     */

/* ensure sieve covers at least 'need' (inclusive) ------------------------ */
static void ensure_limit(idx_t need)
{
    if (need <= global_limit) return;

    idx_t new_limit = need * 2 + 10;             /* safety margin           */
    uint8_t *new_flag = (uint8_t*)calloc((size_t)new_limit + 1, sizeof(uint8_t));
    if (!new_flag) return;                      /* malloc failure, abort   */

    /* pre‑populate prime flags */
    memset(new_flag, 1, (size_t)new_limit + 1);
    new_flag[0] = new_flag[1] = 0;

    /* simple Eratosthenes sieve */
    idx_t sq = (idx_t)sqrt((double)new_limit);
    for (idx_t p = 2; p <= sq; ++p) {
        if (new_flag[p]) {
            for (idx_t k = p * p; k <= new_limit; k += p)
                new_flag[k] = 0;
        }
    }

    /* build composite list */
    size_t new_cap = new_limit / 2 + 16;
    idx_t *new_list = (idx_t*)malloc(new_cap * sizeof(idx_t));
    if (!new_list) {
        free(new_flag);
        return;
    }

    size_t cc = 0;
    for (idx_t v = 4; v <= new_limit; ++v) {
        if (!new_flag[v]) {                     /* composite */
            if (cc == new_cap) {
                new_cap <<= 1;
                idx_t *tmp = (idx_t*)realloc(new_list, new_cap * sizeof(idx_t));
                if (!tmp) break;                 /* allocation failure */
                new_list = tmp;
            }
            new_list[cc++] = v;
        }
    }

    /* replace old state */
    free(prime_flag);
    free(composite_list);
    prime_flag = new_flag;
    composite_list = new_list;
    comp_cap = new_cap;
    comp_cnt = cc;
    global_limit = new_limit;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined, harmless */

    /* start with a reasonable bound: 2*n+10 */
    idx_t bound = n * 2 + 10;
    ensure_limit(bound);

    while (1) {
        bool *seen = (bool*)calloc((size_t)n, sizeof(bool));
        if (!seen) return (seq_t)0;             /* allocation failure */

        size_t found = 0;
        seq_t answer = (seq_t)0;

        for (size_t i = 0; i < comp_cnt; ++i) {
            seq_t c = (seq_t)composite_list[i];
            seq_t r = c % (seq_t)n;
            size_t ridx = (size_t)r;
            if (!seen[ridx]) {
                seen[ridx] = true;
                if (++found == n) {
                    answer = c;
                    break;
                }
            }
        }

        free(seen);

        if (answer != (seq_t)0) return answer;  /* finished */

        /* not all residues seen yet – expand the sieve and try again */
        bound *= 2;
        ensure_limit(bound);
    }
}
