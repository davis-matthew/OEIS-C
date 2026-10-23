/*
 * A161868  Primes p such that both p+2 and p-2 are products of 3 distinct
 *          primes.
 * https://oeis.org/A161868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that p-2 and p+2 are each the
 *            product of exactly 3 distinct primes (each occurring to the
 *            first power)
 * Range:     u8 none, u16 n <= 351, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is performed by on-demand search with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static seq_t *special = NULL;      /* list of primes satisfying the property */
static size_t special_len = 0;      /* number of stored elements */
static size_t special_cap = 0;      /* allocated capacity */
static uint64_t last_checked = 5;  /* largest number examined so far */

/* ------------------------------------------------------------------ */
/* Ensure the special array has at least `needed` elements of space. */
static void ensure_special_cap(size_t needed)
{
    if (needed <= special_cap) return;
    size_t new_cap = special_cap ? special_cap * 2 : 1024;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(special, new_cap * sizeof(seq_t));
    if (tmp) {
        special = tmp;
        special_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Return true iff n is a product of exactly three distinct primes,
   all of exponent one.  All intermediate calculations are done in
   uint64_t because the numbers involved are far below 2^64. */
static bool is_three_distinct(uint64_t n)
{
    /* For n <= 1 product condition cannot hold. */
    if (n < 2) return false;

    uint64_t pr[32], ex[32];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    if (cnt != 3) return false;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] != 1) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Generate special primes up to at least `n` terms.  The search
   proceeds by testing consecutive odd numbers for primality.
   Only primes are examined; each such prime is tested for the
   required property.  Progress continues from where it left off. */
static void generate_up_to(idx_t n)
{
    if ((size_t)n <= special_len) return;   /* already have enough */

    uint64_t candidate = last_checked + 1;
    if ((candidate & 1ULL) == 0) candidate++;   /* make it odd */

    while (special_len < (size_t)n) {
        if (is_prime((idx_t)candidate)) {
            if (candidate > 5) {   /* p-2 >= 3 is necessary */
                uint64_t a = candidate - 2;
                uint64_t b = candidate + 2;
                if (is_three_distinct(a) && is_three_distinct(b)) {
                    ensure_special_cap(special_len + 1);
                    special[special_len++] = (seq_t)candidate;
                }
            }
        }
        last_checked = candidate;
        candidate += 2;   /* next odd number */
    }
}

/* ------------------------------------------------------------------ */
/* OEIS sequence accessor. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, undefined for 0 */
    generate_up_to(n);                  /* ensure we have at least n terms */
    if ((size_t)(n - 1) < special_len)
        return special[n - 1];
    return 0;        /* out of range – should not happen for required n */
}
