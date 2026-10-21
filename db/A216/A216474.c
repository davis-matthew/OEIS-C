/*
 * A216474  Smallest k such that k*!n +1 is an n-th power.
 * https://oeis.org/A216474
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A216474
   A(n) = smallest k such that k*!n + 1 is an n‑th power.
   Here !n denotes the subfactorial (derangement) of n. */

#include <stddef.h>

/* ---------- helpers ---------- */

static seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    if (mod == (seq_t)1) return (seq_t)0;          /* 1 mod 1 is 0 */
    seq_t res = (seq_t)1 % mod;                    /* works for mod > 1 */
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

static seq_t big_pow(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

/* ---------- subfactorial cache ---------- */

#define SUBFACT_MAX 20001          /* enough for n up to ~20000 */
static seq_t subfact[SUBFACT_MAX];
static size_t subfact_len = 0;     /* number of already computed entries */

/* Ensure entries 0..n (inclusive) are computed */
static void ensure_subfactorial(idx_t n)
{
    if (n >= SUBFACT_MAX) return;  /* out of bounds – caller must guard */
    if (n < subfact_len) return;

    if (subfact_len == 0) {
        subfact[0] = (seq_t)1;
        subfact_len = 1;
    }
    if (subfact_len == 1) {
        subfact[1] = (seq_t)0;
        subfact_len = 2;
    }
    for (size_t i = subfact_len; i <= (size_t)n; ++i) {
        subfact[i] = ((seq_t)(i - 1)) * (subfact[i - 1] + subfact[i - 2]);
    }
    subfact_len = (size_t)n + 1;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < (idx_t)2) return (seq_t)0;      /* below offset – undefined */

    if (n >= SUBFACT_MAX) return (seq_t)0; /* safety guard */

    ensure_subfactorial(n);
    seq_t subf = subfact[n];               /* !n */

    /* Special case: !n == 1 (only n = 2).  Then y = 2 is forced. */
    if (subf == (seq_t)1) {
        return big_pow((seq_t)2, n) - (seq_t)1;
    }

    /* Find minimal y > 1 such that y^n ≡ 1 (mod !n) */
    seq_t y_val = 0;
    for (idx_t y = 2; ; ++y) {
        if (mod_pow((seq_t)y, n, subf) == (seq_t)1) {
            y_val = (seq_t)y;
            break;
        }
    }

    /* Compute k = (y^n - 1) / !n */
    seq_t pow_y = big_pow(y_val, n);
    return (pow_y - (seq_t)1) / subf;
}
