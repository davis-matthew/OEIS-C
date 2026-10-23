/*
 * A174470  G.F.: exp( Sum_{n>=1} A014578(n)*(3x)^n/n ), where A014578 is the
 *          binary expansion of Thue constant.
 * https://oeis.org/A174470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174470
   Function: a(n) = coefficient of x^n in
                exp( Σ_{k≥1} (A014578(k) * (3x)^k / k) )
   where A014578(k) is the k‑th digit of the Thue–Morse constant
   in binary: 1 except when k ≡ 3 or 6 (mod 9) – then 0.
   The implementation uses the explicit recurrence
          k * a(k) = Σ_{i=0}^{k-1} a(i) * c(k-1-i),
   with   c(m) = 3^(m+1) * A014578(m+1).
   Memoized to handle out‑of‑order calls.      */

#include <stdlib.h>
#include <stdint.h>

/* External types supplied by the build */
extern seq_t a(idx_t n);

static seq_t *a_cache = NULL;   /* a[0..len-1] */
static size_t a_len = 0, a_cap = 0;

static seq_t *c_cache = NULL;   /* c[0..len-1] */
static size_t c_len = 0, c_cap = 0;

static seq_t pow3 = 0;          /* 3^(next_index)  */
static int initialized = 0;

/* Ensure a_cache and c_cache contain values up to index n (inclusive) */
static void ensure(idx_t n)
{
    if (!initialized) {
        /* initialise for a(0) = 1 */
        a_cap = 4;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_len = 1;

        c_cache = NULL;
        c_len = 0;
        c_cap = 0;

        pow3 = (seq_t)3;          /* 3^(1) for the first term */
        initialized = 1;
    }

    if (a_len > n) return;              /* already computed up to n */

    for (idx_t k = a_len; k <= n; ++k) {
        /* digit of Thue–Morse constant: 0 iff k ≡ 3 or 6 (mod 9) */
        seq_t d = ((k % 9 == 3) || (k % 9 == 6)) ? 0 : 1;

        /* ensure capacity for c_cache */
        if (c_len == c_cap) {
            c_cap = c_cap ? c_cap * 2 : 4;
            c_cache = (seq_t *)realloc(c_cache, c_cap * sizeof(seq_t));
        }
        seq_t ck1 = pow3 * d;           /* c(k-1) */
        c_cache[c_len++] = ck1;

        /* compute a(k) */
        seq_t sum = 0;
        for (idx_t i = 0; i < k; ++i) {
            sum += a_cache[i] * c_cache[k - 1 - i];
        }
        seq_t ak = sum / k;             /* division exact */

        /* ensure capacity for a_cache */
        if (a_len == a_cap) {
            a_cap = a_cap * 2;
            a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
        }
        a_cache[a_len++] = ak;

        pow3 *= (seq_t)3;                /* next power of 3 */
    }
}

seq_t a(idx_t n)
{
    ensure(n);
    return a_cache[n];
}
