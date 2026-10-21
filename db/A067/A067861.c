/*
 * A067861  n is prime and is the concatenation of numbers n_1, n_2, n_3, in
 *          that order, with n_1 - n_2 = n_3. (Do not allow leading zeros for
 *          nonzero n_i.)
 * https://oeis.org/A067861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   equal_strings
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A067861: a(n) is the n-th prime p that can be written as
 *  the concatenation of three numbers n1 n2 n3 (no leading zeros for
 *  non‑zero parts) with n1 – n2 = n3.
 *  Algorithm: sequentially test primes until the n‑th satisfying prime
 *  is found.  The prime test uses deterministic Miller–Rabin for 64‑bit
 *  integers; the concatenation check is performed by examining all
 *  possible split positions of the decimal representation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/equal_strings.h"

/* ---------- 64‑bit Miller–Rabin primality test ---------- */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] =
        {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    size_t i;
    for (i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2,325,9375,28178,450775,9780504,1795265022};
    for (i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------  helper string utilities  ---------- */
static void string_subtract(const char *a, int len_a,
                            const char *b, int len_b,
                            char *out, int *len_out)
{
    /* assume a >= b numerically, both in decimal without leading zeros */
    int i = len_a - 1, j = len_b - 1, k = len_a - 1;
    int borrow = 0;
    while (i >= 0)
    {
        int da = a[i] - '0';
        int db = (j >= 0) ? b[j] - '0' : 0;
        int d = da - db - borrow;
        if (d < 0) { d += 10; borrow = 1; }
        else borrow = 0;
        out[k--] = (char)(d + '0');
        --i; --j;
    }
    /* strip leading zeros */
    int pos = 0;
    while (pos < len_a - 1 && out[pos] == '0') ++pos;
    *len_out = len_a - pos;
    if (pos > 0) memmove(out, out + pos, *len_out);
}

/* ----------  test whether prime satisfies concatenation property ---------- */
static bool satisfies(uint64_t prime)
{
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%llu", (unsigned long long)prime);
    if (len < 3) return false;   /* at least three digits needed */

    for (int i = 1; i <= len - 2; ++i)
    {
        for (int j = i + 1; j <= len - 1; ++j)
        {
            int len_a = i, len_b = j - i, len_c = len - j;
            /* leading zero rule */
            if (len_b > 1 && buf[i] == '0') continue;
            if (len_c > 1 && buf[j] == '0') continue;

            /* a >= b? else difference would be negative */
            bool a_ge_b = true;
            if (len_a > len_b) a_ge_b = true;
            else if (len_a < len_b) a_ge_b = false;
            else
                a_ge_b = (strncmp(buf, buf + i, len_a) >= 0);

            if (!a_ge_b) continue;

            /* compute a - b */
            char diff[32];
            int difflen;
            string_subtract(buf, len_a, buf + i, len_b, diff, &difflen);

            /* compare diff to c */
            if (equal_strings(diff, difflen, buf + j, len_c))
                return true;
        }
    }
    return false;
}

/* ----------  main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 but 0 is unused */

    uint64_t cnt = 0;
    uint64_t candidate = 2;
    while (cnt < n)
    {
        if (isPrime64(candidate) && satisfies(candidate))
        {
            ++cnt;
            if (cnt == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
    /* unreachable, but return 0 for completeness */
    return (seq_t)0;
}
