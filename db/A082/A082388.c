/*
 * A082388  a(1) = 1, a(2) = 2; further terms are defined by rules that for k
 *          >= 2, a(2^k-i) = a(2^k+i) for 1 <= i <= 2^k-1 and a(2^k) =
 *          a(2^(k-1)) + Sum_{i < 2^k} a(i).
 * https://oeis.org/A082388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1023, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=2;
   for k≥2: a(2^k‑i)=a(2^k+i)  (1≤i≤2^k‑1)
   and a(2^k)=4·a(2^(k‑1))–2·a(2^(k‑2))  */

/* ------------------------------------------------------------ */
#include <stdint.h>
#include <stdlib.h>

/* Helper: floor(log2(x)) for unsigned 64‑bit integer */
static inline int _floor_log2_uint64(idx_t x)
{
    int r = 0;
    while (x >>= 1)
        ++r;
    return r;
}

/* Memoisation cache ------------------------------------------------- */
static seq_t *a_cache = NULL;   /* a_cache[i] holds a(i+1)       */
static size_t  a_cache_size = 0; /* current capacity in elements  */

static void ensure_cache(idx_t size)
{
    if (size <= a_cache_size)
        return;

    size_t new_cap = a_cache_size ? a_cache_size << 1 : 16;
    while (new_cap < (size_t)size)
        new_cap <<= 1;

    seq_t *newp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort();           /* out of memory */

    /* initialise new entries to 0 (unknown) */
    for (size_t i = a_cache_size; i < new_cap; ++i)
        newp[i] = (seq_t)0;

    a_cache = newp;
    a_cache_size = new_cap;
}

/* Test whether n is a power of two (n>0) */
static inline int is_pow2(idx_t n) { return n && !(n & (n - 1)); }

/* Forward declaration of the recursive routine */
static seq_t compute_a(idx_t n);

/* ---------------------------------------------------------- public function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined for the sequence, but safe          */

    ensure_cache(n);

    seq_t val = a_cache[n - 1];
    if (val != (seq_t)0)         /* already computed */
        return val;

    val = compute_a(n);

    a_cache[n - 1] = val;        /* memoise */
    return val;
}

/* -------------------------------------------------------------- recursive helper */
static seq_t compute_a(idx_t n)
{
    /* base cases */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    /* power of two case */
    if (is_pow2(n)) {
        int k = _floor_log2_uint64(n);
        /* k>=1 because n>=2 */
        seq_t a_k1 = compute_a((idx_t)1ULL << (k - 1)); /* 2^(k-1) */
        seq_t a_k2 = compute_a((idx_t)1ULL << (k - 2)); /* 2^(k-2) */
        return (seq_t)(4 * a_k1 - 2 * a_k2);
    }

    /* mirror symmetry: find the closest lower power of two */
    int k = _floor_log2_uint64(n);         /* k >= 0, pow2 <= n  */
    idx_t pow2 = (idx_t)1ULL << k;        /* lower power of two  */
    idx_t partner = (pow2 << 1) - n;       /* 2*pow2 - n, < pow2 */
    return compute_a(partner);             /* partner < n, guarantees termination */
}
