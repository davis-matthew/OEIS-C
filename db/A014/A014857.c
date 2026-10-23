/*
 * A014857  Numbers k that divide s(k), where s(1)=1, s(j)=9*s(j-1)+j.
 * https://oeis.org/A014857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k for which k divides s(k), where s(1)=1
 *            and s(j)=9*s(j-1)+j. Equivalently, s(k)=∑_{i=1}^{k} i·9^{k-i}. k
 *            divides s(k) ⇔ ∑_{i=0}^{k-1} i·9^{i} ≡ 0 (mod k)
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* types seq_t and idx_t are defined by the build environment */

/* ---------- auxiliary for modular multiplication ---------- */
static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    return ( unsigned __int128 )a * b % mod;
#else
    uint64_t res = 0;
    a %= mod;
    while (b) {
        if (b & 1ULL) {
            res += a;
            if (res >= mod) res -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
#endif
}

/* ---------- compute S(k) = ∑_{i=0}^{k-1} i·9^{i} (mod k) ---------- */
static uint64_t modular_sum(uint64_t k)
{
    if (k <= 1) return 0;          /* S(1)=0, special case */
    uint64_t mod = k;
    uint64_t pow = 1 % mod;        /* 9^0 */
    uint64_t sum = 0;
    for (uint64_t i = 0; i < mod; ++i) {
        uint64_t prod = mul_mod_u64(i, pow, mod);
        sum += prod;
        if (sum >= mod) sum -= mod;
        pow = mul_mod_u64(pow, 9, mod);   /* pow = 9^{i+1} mod k */
    }
    return sum % mod;
}

/* ---------- dynamic array to store the sequence ---------- */
static seq_t *seq_array = NULL;
static size_t seq_count  = 0;   /* number of terms currently stored */
static size_t seq_cap    = 0;   /* allocated capacity */
static uint64_t candidate = 1;  /* next integer to test */

/* make sure the array can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_array = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* ---------- public function implementing A014857 ---------- */
seq_t a(idx_t n)
{
    /* produce terms until we have at least n of them */
    while (seq_count < (size_t)n) {
        uint64_t k = candidate++;
        if (modular_sum(k) == 0ULL) {          /* k divides s(k) */
            ensure_capacity(seq_count + 1);
            seq_array[seq_count++] = (seq_t)k;
        }
    }
    return seq_array[n - 1];
}
