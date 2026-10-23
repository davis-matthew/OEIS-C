/*
 * A160819  a(1)=2. a(n) = smallest positive integer such that Omega(a(n)) =
 *          Omega(sum{k=1 to n} a(k)), where Omega(m) = A001222(m).
 * https://oeis.org/A160819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160819
   a(1) = 2
   For n > 1: a(n) = smallest positive integer x such that
   Omega(x) = Omega( Sum_{k=1..n} a(k) ), where Omega(m) = A001222(m) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

static seq_t *a_values = NULL;   /* a[1] .. a[max_n]  */
static seq_t *cum_values = NULL; /* cumulative sums cum[1] .. cum[max_n]  */
static size_t alloc_cap = 0;
static idx_t last_computed = 0;

/* Ensure the internal tables are large enough for indices up to n (inclusive). */
static void grow_to(idx_t n)
{
    if (n <= alloc_cap) return;
    size_t new_cap = alloc_cap ? alloc_cap * 2 : 4;
    while (new_cap < (size_t)n) new_cap *= 2;
    a_values = seq_realloc(a_values, alloc_cap, new_cap);
    cum_values = seq_realloc(cum_values, alloc_cap, new_cap);
    alloc_cap = new_cap;
}

/* Return the smallest x such that Omega(x) == Omega(sum + x). */
static seq_t find_next_term(seq_t sum)
{
    seq_t x = 2;                      /* 1 cannot satisfy the condition (Omega(1)=0) */
    for (;;) {
        seq_t ox = count_prime_factors(x);
        seq_t os = count_prime_factors(sum + x);
        if (ox == os) return x;
        ++x;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n=0, return 0 as fallback */

    grow_to((idx_t)n + 1);            /* arrays are 1‑indexed; index 0 unused */

    if (n > last_computed) {
        seq_t sum = (last_computed > 0) ? cum_values[last_computed] : 0;
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t term = find_next_term(sum);
            a_values[i] = term;
            sum += term;
            cum_values[i] = sum;
        }
        last_computed = n;
    }
    return a_values[n];
}
