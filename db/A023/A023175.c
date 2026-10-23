/*
 * A023175  Numbers k such that Fibonacci(k) == 3 (mod k).
 * https://oeis.org/A023175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023175: Numbers k such that Fibonacci(k) ≡ 3 (mod k).
 * a(n) returns the n‑th such k, with a(1)=1, a(2)=2, a(3)=4, …  */
/* The function builds the sequence lazily; it is safe to call with
   arbitrary order of arguments.  All intermediate values fit in
   64 bits because k is always small ( < 2^16 for n up to 10000 ),
   so 128‑bit arithmetic is used only to avoid overflow. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *terms = NULL;          /* stored sequence terms */
static size_t terms_len = 0;          /* number of terms stored */
static size_t terms_cap = 0;          /* allocated capacity */
static idx_t next_k = 1;              /* next k to test */

/* Ensure capacity for at least 'need' elements. */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!p) abort();
        terms = p;
        terms_cap = new_cap;
    }
}

/* Fast‑doubling algorithm that returns (F(n), F(n+1)) modulo 'mod'.
 * All arithmetic performed as uint64_t with 128‑bit intermediate to
 * avoid overflow.  The function is recursive with depth O(log n). */
static void fib_pair_mod_rec(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    uint64_t f, f1;
    fib_pair_mod_rec(n >> 1, mod, &f, &f1);

    /* Compute c = F(2m) and d = F(2m+1) modulo mod */
    const uint64_t two_f1 = (f1 << 1) % mod;           // 2*F(m+1) mod mod
    const uint64_t diff = (two_f1 >= f) ? two_f1 - f : two_f1 + mod - f;
    const uint64_t c = ( (__uint128_t)f * diff ) % mod;
    const uint64_t d = ( ( (__uint128_t)f * f ) % mod
                       + ( (__uint128_t)f1 * f1 ) % mod ) % mod;

    if ((n & 1) == 0) {     /* n = 2m */
        *fn  = c;
        *fn1 = d;
    } else {                /* n = 2m+1 */
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* Return Fibonacci(n) mod n using the above routine. */
static uint64_t fib_mod_uint64(idx_t n)
{
    if (n == 0) return 0;
    uint64_t fn, fn1;
    fib_pair_mod_rec((uint64_t)n, (uint64_t)n, &fn, &fn1);
    return fn;
}

/* Append a newly found term. */
static void add_term(idx_t k)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = (seq_t)k;
}

/* Main entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */

    while (terms_len < (size_t)n) {
        idx_t k = next_k++;                /* candidate k */
        uint64_t fk_mod = fib_mod_uint64(k);     /* Fibonacci(k) mod k */
        uint64_t rem3   = 3 % k;                /* 3 mod k   */
        if (fk_mod == rem3) add_term(k);
    }
    return terms[n - 1];
}
