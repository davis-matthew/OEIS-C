/*
 * A082378  Positive integers that can be written in exactly one way as the
 *          product ab of positive integers where a+b is composite.
 * https://oeis.org/A082378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 7849, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A082378: Positive integers that can be written in exactly one way as
   a product ab of positive integers with a+b composite.
   This implementation tests each integer in increasing order until the
   required number of terms have been found.  For each candidate n it
   enumerates all divisor pairs (a,b) with a ≤ b, counts those whose
   sum a+b is composite, and keeps n if the count equals one.  The
   resulting values are stored in a static array for fast subsequent
   access.  The routine works for any seq_t and idx_t supplied by the
   build. */

/* ---------- include required headers ---------- */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- helper: primality test for 64‑bit values ---------- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    /* trial division up to sqrt(n) */
    for (uint64_t d = 5; d * d <= n; d += 6) {
        if (n % d == 0)   return false;
        if (n % (d + 2) == 0) return false;
    }
    return true;
}

/* ---------- sequence storage ---------- */
static seq_t *terms = NULL;   /* array of sequence terms */
static size_t terms_cap = 0;
static size_t terms_len = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

/* enlarge terms array if needed */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* generate terms up to at least 'target' entries */
static void generate_terms(idx_t target)
{
    while (terms_len < (size_t)target) {
        uint64_t n = next_candidate;
        size_t composite_pairs = 0;

        /* loop over divisor pairs a ≤ b */
        for (uint64_t a = 1; a * a <= n; ++a) {
            if (n % a != 0) continue;
            uint64_t b = n / a;
            uint64_t sum = a + b;
            if (sum <= 1) continue;                 /* sum is always >=2, safety */
            if (!is_prime_u64(sum)) {                /* composite sum ? */
                ++composite_pairs;
                if (composite_pairs > 1) break;      /* early exit */
            }
        }

        if (composite_pairs == 1) {                  /* exactly one composite pair */
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)n;
        }
        ++next_candidate;
    }
}

/* ---------- public function ---------- */
extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */
    generate_terms(n);
    return terms[n - 1];
}
