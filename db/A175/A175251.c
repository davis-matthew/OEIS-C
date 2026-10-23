/*
 * A175251  Composites (A002808) with nonprime (A018252) subscripts.
 * https://oeis.org/A175251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   grow_seq, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = composite(nonprime(n))
 * where nonprime(n) is the n‑th nonprime number (1,4,6,8,9,10,…)
 * and composite(k) is the k‑th composite number (4,6,8,9,10,…).
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static idx_t *nonprimes   = NULL;   /* nonprimes[i] = nonprime(i+1) */
static size_t nonprimes_cnt = 0;
static size_t nonprimes_cap = 0;
static idx_t  next_nonprime = 1;     /* next integer to test for nonprime */

static seq_t *composites   = NULL;   /* composites[i] = composite(i+1) */
static size_t composites_cnt = 0;
static size_t composites_cap = 0;
static idx_t  next_composite = 4;    /* next integer to test for composite */

/* Grow an array of idx_t values. */
static void
grow_idx(idx_t **p, size_t *cap, size_t new_cap)
{
    if (*cap < new_cap) {
        idx_t *tmp = (idx_t *)realloc(*p, new_cap * sizeof(idx_t));
        if (tmp == NULL) abort();
        *p   = tmp;
        *cap = new_cap;
    }
}

/* Ensure that we have at least `need` nonprimes stored. */
static void
ensure_nonprimes(idx_t need)
{
    while (nonprimes_cnt < (size_t)need) {
        idx_t x = next_nonprime;
        if (x == 1) {
            grow_idx(&nonprimes, &nonprimes_cap, nonprimes_cnt + 1);
            nonprimes[nonprimes_cnt++] = x;
        } else {
            if (!is_prime_small((seq_t)x)) {
                grow_idx(&nonprimes, &nonprimes_cap, nonprimes_cnt + 1);
                nonprimes[nonprimes_cnt++] = x;
            }
        }
        ++next_nonprime;
    }
}

/* Ensure that we have at least `need` composites stored. */
static void
ensure_composites(idx_t need)
{
    while (composites_cnt < (size_t)need) {
        idx_t x = next_composite;
        if (x >= 4) {
            if (!is_prime_small((seq_t)x)) {
                grow_seq(&composites, &composites_cap, composites_cnt + 1);
                composites[composites_cnt++] = (seq_t)x;
            }
        }
        ++next_composite;
    }
}

/* Compute the OEIS sequence A175251. */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    ensure_nonprimes(n);
    idx_t np = nonprimes[n - 1];          /* n‑th nonprime */

    /* Ensure composites up to index np exist. */
    if (np == 0) return (seq_t)0; /* defensive, shouldn't happen */
    ensure_composites(np);
    return composites[(size_t)(np - 1)];
}
