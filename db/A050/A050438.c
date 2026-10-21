/*
 * A050438  Fourth-order composites.
 * https://oeis.org/A050438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the 4th‐order composite:  a(n) = C(C(C(C(n)))) ,
   where C(k) is the k-th composite number (C(1)=4) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: simple primality test for uint64_t (works for small values) */
static bool is_prime_uint64(uint64_t num)
{
    if (num < 2)          return false;
    if (num % 2 == 0)     return num == 2;
    for (uint64_t d = 3; d <= num / d; d += 2) {
        if (num % d == 0) return false;
    }
    return true;
}

/* --- Cached list of composite numbers --- */
static seq_t   *composites = NULL;
static size_t   comp_len   = 0;
static size_t   comp_cap   = 0;
static uint64_t checked_upto = 3ULL;   /* last integer examined */

/* Ensure that at least 'need' composite numbers are stored. */
static void ensure_composites(idx_t need)
{
    if (comp_len >= need) return;

    if (comp_cap == 0) {
        comp_cap    = 64;
        composites  = (seq_t*)calloc(comp_cap, sizeof(seq_t));
    }

    uint64_t last_checked = checked_upto;
    for (uint64_t num = last_checked + 1; comp_len < need; ++num) {
        if (num > 3 && !is_prime_uint64(num)) {
            if (comp_len == comp_cap) {
                comp_cap *= 2;
                composites = (seq_t*)realloc(composites, comp_cap * sizeof(seq_t));
            }
            composites[comp_len++] = (seq_t)num;
            last_checked = num;
        }
    }
    checked_upto = last_checked;
}

/* Return the k-th composite number (k ≥ 1). */
static void kth_composite(idx_t k, seq_t *out)
{
    ensure_composites(k);
    *out = composites[k - 1];
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t val, next;

    kth_composite(n, &val);            /* C(n)   */
    kth_composite((idx_t)val, &next);
    val = next;                        /* C(C(n)) */
    kth_composite((idx_t)val, &next);
    val = next;                        /* C(C(C(n))) */
    kth_composite((idx_t)val, &next);
    val = next;                        /* C(C(C(C(n)))) */
    return val;
}
