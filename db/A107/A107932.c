/*
 * A107932  Numbers n such that n and n-th prime have only one common digit =
 *          1.
 * https://oeis.org/A107932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A107932: Numbers n such that n and the n‑th prime share only the digit 1.
   The function returns the n‑th term of the sequence (offset 1).  It generates
   primes lazily using Miller–Rabin for 64‑bit integers and stores the sequence
   in a static array.  Digits are handled with a 16‑bit mask.  All arithmetic
   uses only integer operators as required. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- 64‑bit modular multiplication and power ----------
   (safe against overflow for 64‑bit operands)                     */
static inline uint64_t mul64_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}
static uint64_t pow64_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1) res = mul64_mod(res, base, mod);
        base = mul64_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* ---------- Miller–Rabin primality test for 64‑bit integers ----------
   Uses deterministic bases: 2,3,5,7,11,13,17,19,23,29,31,37   */
static bool isPrime64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    if (n % 3 == 0)           return n == 3;
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) break;
        uint64_t x = pow64_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul64_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Digit mask: 16‑bit bitmap of digits present ----------
   Bit i set if digit i occurs somewhere in the number.           */
static inline uint16_t digit_mask_uint64(uint64_t x)
{
    uint16_t m = 0;
    if (x == 0) return 1u;          /* digit 0 only */
    while (x) {
        m |= 1u << (uint16_t)(x % 10);
        x /= 10;
    }
    return m;
}

/* ---------- Prime storage ----------
   primes are stored in a dynamic array of uint64_t                */
static uint64_t *primebuf = NULL;
static size_t primecap = 0, primecnt = 0;

static void ensure_prime_cap(size_t need)
{
    if (primecap < need) {
        size_t newcap = primecap ? primecap * 2 : 16;
        while (newcap < need) newcap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(primebuf, newcap * sizeof(uint64_t));
        if (!tmp) exit(1);
        primebuf = tmp;
        primecap = newcap;
    }
}

static void ensure_prime_index(size_t idx)   /* idx >= 1 */
{
    while (primecnt < idx) {
        uint64_t cand = (primecnt == 0) ? 2 : primebuf[primecnt - 1] + 1;
        while (!isPrime64(cand)) ++cand;
        ensure_prime_cap(primecnt + 1);
        primebuf[primecnt++] = cand;
    }
}

/* ---------- Sequence storage ----------
   seqbuf holds the computed terms; indices are 0‑based for array storage. */
static seq_t *seqbuf = NULL;
static size_t seqcap = 0, seqlen = 0;   /* number of terms stored */

/* static variable to remember the next candidate number to test   */
static uint64_t candidate = 1;          /* candidates are tested in ascending order */

static void ensure_seq_cap(size_t need)
{
    if (seqcap < need) {
        size_t newcap = seqcap ? seqcap * 2 : 16;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seqbuf, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        seqbuf = tmp;
        seqcap = newcap;
    }
}

static void generate_terms(idx_t need)   /* need >= 1 */
{
    while (seqlen < (size_t)need) {
        /* ensure we know the candidate‑th prime */
        ensure_prime_index(candidate);

        uint64_t prime = primebuf[candidate - 1];   /* primes are 0‑based   */
        uint16_t m1 = digit_mask_uint64(candidate);
        uint16_t m2 = digit_mask_uint64(prime);
        uint16_t inter = m1 & m2;

        if (inter == (1u << 1)) {   /* only digit 1 common */
            ensure_seq_cap(seqlen + 1);
            seqbuf[seqlen++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* ---------- public function  ------------------------------------- */
seq_t a(idx_t n)         /* n is 1‑based index */
{
    if (n == 0) return 0;  /* undefined; return 0 for safety */
    generate_terms(n);
    return seqbuf[n - 1];
}
