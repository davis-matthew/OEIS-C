/*
 * A051854  Table of solutions to all possible Chinese Remainder Equations x =
 *          a1 mod p1, x = a2 mod p2, ..., x = an mod pn, where p1 - pn are
 *          the first n primes and each a1 - an varies between 1 and (its
 *          respective) p-1, with the rightmost a (an) varying fastest.
 * https://oeis.org/A051854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = solution of the Chinese Remainder system for n-th
 *            enumeration over primes 2,3,5,7,... with residues ranging from
 *            1..p-1 (rightmost prime varying fastest)
 * Range:     u8 n <= 61, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

/* Modular inverse modulo a prime modulus */
static seq_t modinv(seq_t a, seq_t mod)
{
    seq_t a_mod = a % mod;
    uint64_t m = (uint64_t)mod;
    return mod_pow(a_mod, m - 2, mod);
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; n==0 is undefined but harmless */
        return 0;

    /* ---------- smallest required primes (pre‑computed, static) ---------- */
    /* First 100 primes are more than enough for the intended n range */
    static uint32_t primes[100];
    static size_t prime_cnt = 0;
    if (prime_cnt == 0) {
        size_t cnt = 0;
        for (uint32_t p = 2; cnt < 100; ++p) {
            int prime = 1;
            for (size_t i = 0; i < cnt; ++i) {
                if (p % primes[i] == 0) {
                    prime = 0;
                    break;
                }
                if ((uint64_t)primes[i] * primes[i] > p)
                    break;
            }
            if (prime)
                primes[cnt++] = p;
        }
        prime_cnt = cnt;
    }

    /* ---------- locate the row containing n ---------- */
    idx_t sum = 0;          /* cumulative number of terms in previous rows */
    idx_t rowLen = 1;       /* product of (p_i-1) for current row */
    size_t r = 0;           /* number of primes processed so far */

    while (1) {
        idx_t rad = (idx_t)primes[r] - 1;          /* radix for this prime */
        idx_t nextLen = rowLen * rad;              /* length of the next row */

        if (sum + nextLen >= n)
            break;                                  /* n lies in this row */

        sum += nextLen;
        rowLen = nextLen;
        ++r;

        /* safety check: we have enough primes pre‑generated */
        if (r >= prime_cnt)
            break;
    }
    ++r; /* r is the count of primes in the desired row (1‑based) */

    /* ---------- compute local index within the row ---------- */
    idx_t k = n - sum - 1;          /* zero‑based index in this row */

    /* ---------- residue array (a_i) and modulus array (p_i) ---------- */
    seq_t residues[100];
    seq_t mods[100];
    for (size_t i = 0; i < r; ++i)
        mods[i] = (seq_t)primes[i];

    /* ---------- mixed‑radix decomposition (rightmost changes fastest) ---------- */
    for (size_t i = r; i > 0; --i) {
        idx_t rad   = (idx_t)primes[i - 1] - 1;
        idx_t digit = k % rad;
        k          /= rad;
        residues[i - 1] = (seq_t)(digit + 1); /* ai in [1, p_i-1] */
    }

    /* ---------- Garner’s algorithm to reconstruct the CRT solution ---------- */
    seq_t x     = 0;
    seq_t coeff = 1;

    for (size_t i = 0; i < r; ++i) {
        seq_t mi   = mods[i];
        seq_t ri   = residues[i];

        seq_t diff = (ri - (x % mi) + mi) % mi;
        seq_t inv  = modinv(coeff % mi, mi);
        seq_t yi   = (diff * inv) % mi;

        x     = x + coeff * yi;
        coeff = coeff * mi;
    }

    return x;
}
