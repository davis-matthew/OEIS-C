/*
 * A073425  a(0)=0; for n>0, a(n) = number of primes not exceeding n-th
 *          composite number.
 * https://oeis.org/A073425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n‑th composite number) – n – 1, a(0)=0 (A073425). The
 *            sequence is computed by generating the first n composites with a
 *            simple sieve and then using the direct formula. The
 *            implementation works for the GNU C++ build where seq_t is an
 *            arbitrary‑precision integer class; the arrays of idx_t st...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Arrays of integer values used to generate primes and composites. */
static idx_t *prime_list = NULL;
static size_t  prime_cnt  = 0;
static size_t  prime_cap  = 0;

static idx_t *comp_list  = NULL;
static size_t  comp_cnt   = 0;
static size_t  comp_cap   = 0;

static idx_t last_checked = 2;          /* last integer examined while generating */

static void ensure_cap(idx_t **arr, size_t *cnt, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = (idx_t *)realloc(*arr, new_cap * sizeof(idx_t));
        *cap = new_cap;
    }
}

static void init_prime_list(void)
{
    if (prime_cnt == 0) {
        ensure_cap(&prime_list, &prime_cnt, &prime_cap, 1);
        prime_list[prime_cnt++] = 2;
    }
}

static void ensure_composites(idx_t n_needed)
{
    if (n_needed <= comp_cnt) return;
    init_prime_list();

    while (comp_cnt < n_needed) {
        ++last_checked;
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p * p > last_checked) break;
            if (last_checked % p == 0) { isprime = false; break; }
        }

        if (isprime) {
            ensure_cap(&prime_list, &prime_cnt, &prime_cap, prime_cnt + 1);
            prime_list[prime_cnt++] = last_checked;
        } else {
            ensure_cap(&comp_list, &comp_cnt, &comp_cap, comp_cnt + 1);
            comp_list[comp_cnt++] = last_checked;
        }
    }
}

/* Required function signature */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute first n composites if necessary. */
    ensure_composites(n);

    /* comp_list[0] is the first composite (4). */
    idx_t composite_n = comp_list[n - 1];      /* n‑th composite, 1‑based */

    seq_t result = (seq_t)composite_n - (seq_t)n - (seq_t)1;
    return result;
}
