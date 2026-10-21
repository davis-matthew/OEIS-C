/*
 * A205387  The index j<k such that n divides s(k)-s(j) for some j, where
 *          s(j)=(1/2)C(2j,j).
 * https://oeis.org/A205387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the first index j<k such that n | (s(k)-s(j)), * where
 *            s(j) = (1/2)*C(2j,j)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence
 *   s_k = s_{k-1} * (4k-2) / k
 *   with s_1 = 1.
 * The integer value s_k is kept exactly in a small big‑integer
 * struct sufficient for k ≤ 10000 (≈20000 bits).  For each k we
 * compute s_k modulo n, store the first occurrence of each
 * remainder, and when a remainder repeats we return the first
 * occurrence as a(n).  The algorithm runs in O(n) time and O(n)
 * memory for the remainder table. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Small big‐integer: little‑endian 32‑bit limbs (up to ~20000 bits). */
typedef struct {
    uint32_t d[800];  /* limbs */
    size_t len;       /* number of used limbs */
} BigInt;

/* initialise to 1 */
static void big_init(BigInt *a)
{
    a->len = 1;
    a->d[0] = 1;
}

/* multiply a by a small integer m (exact). */
static void big_mul_small(BigInt *a, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)prod;
        carry   = prod >> 32;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
        if (carry >> 32)
            a->d[a->len++] = (uint32_t)(carry >> 32);
    }
}

/* exact integer division of a by the small integer d (a is a multiple
   of d).  Dividend processed from most significant limb. */
static void big_div_small_exact(BigInt *a, uint32_t d)
{
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0; ) {
        uint64_t cur = (rem << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur / d);
        rem     = cur % d;
    }
    /* remove any leading zero limbs */
    while (a->len > 1 && a->d[a->len - 1] == 0)
        --a->len;
}

/* remainder of a modulo a small 32‑bit modulus. */
static uint32_t big_mod_small(const BigInt *a, uint32_t mod)
{
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0; ) {
        rem = ((rem << 32) | a->d[i]) % mod;
    }
    return (uint32_t)rem;
}

/* --------------------------------------------------------------------- */
/* main sequence function */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside of offset */

    /* table of first occurrence of each remainder (0 … n‑1). */
    size_t *first = (size_t *)calloc((size_t)n, sizeof(size_t));
    if (!first) return (seq_t)0;          /* allocation failure */

    BigInt cur;
    big_init(&cur);                       /* s_1 = 1 */

    for (idx_t k = 1; k <= n + 1; ++k) {
        if (k > 1) {
            /* s_k = s_{k-1} * (4k-2) / k */
            uint32_t mul = 4 * (uint32_t)k - 2;
            big_mul_small(&cur, mul);
            big_div_small_exact(&cur, (uint32_t)k);
        }

        uint32_t rem = big_mod_small(&cur, (uint32_t)n);
        size_t idx  = (size_t)rem;
        if (!first[idx]) {
            first[idx] = (size_t)k;        /* record first appearance */
        } else {
            size_t j = first[idx];
            free(first);
            return (seq_t)j;               /* j found */
        }
    }

    /* The pigeonhole principle guarantees a repeat by k = n+1, so
       this point should never be reached.  We return 1 as a safe
       fallback. */
    free(first);
    return (seq_t)1;
}
