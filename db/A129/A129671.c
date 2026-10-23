/*
 * A129671  Right truncatable primes in base 5 (written in decimal form).
 * https://oeis.org/A129671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   append_candidate, cmp_seq_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute all right‑truncatable primes in base‑5 (written in decimal)
   by depth‑first search of the base‑5 tree. The resulting list is
   sorted increasingly, and a(idx_t n) returns the nth element (1‑based). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"
#include "../utils/cmp_seq_seq.h"

/* Static storage for the finished sequence. */
static seq_t  *a_values      = NULL;
static size_t a_count        = 0;   /* number of stored numbers */
static size_t a_capacity     = 0;   /* allocated capacity */
static bool    a_initialized = false;

/* Depth‑first search: try to append each base‑5 digit (0‥4) to the
   current value.  If the result is prime, add it to the list and
   continue the search from that value. */
static void dfs(seq_t val)
{
    for (int d = 0; d <= 4; ++d) {
        seq_t new_val = val * (seq_t)5 + (seq_t)d;
        if (is_prime_big(new_val)) {
            append_candidate(&a_values, &a_count, &a_capacity, new_val);
            dfs(new_val);
        }
    }
}

/* Build the full finite set of right‑truncatable primes in base‑5.
   The one‑digit base‑5 primes are the only entry points of the search. */
static void build_sequence(void)
{
    seq_t first[2] = {(seq_t)2, (seq_t)3};

    for (size_t i = 0; i < 2; ++i) {
        seq_t v = first[i];
        append_candidate(&a_values, &a_count, &a_capacity, v);
        dfs(v);
    }

    /* Sort the array into increasing order. */
    qsort(a_values, a_count, sizeof(seq_t), cmp_seq_seq);
}

/* Public interface: a(n) returns the nth term (1‑based) of A129671. */
seq_t a(idx_t n)
{
    if (!a_initialized) {
        build_sequence();
        a_initialized = true;
    }

    if (n == 0)                /* undefined for n = 0, return 0 */
        return (seq_t)0;
    if ((idx_t)(n - 1) >= a_count)
        return (seq_t)0;        /* n larger than the number of known terms */

    return a_values[n - 1];
}
