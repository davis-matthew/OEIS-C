/*
 * A067872  Least m > 0 for which m*n^2 + 1 is a square.
 * https://oeis.org/A067872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, changed
 * Formula:   a(n) = least m > 0 such that m*n^2 + 1 is a square. Computed by
 *            factorising n and solving s^2 ≡ 1 (mod n^2) via CRT. Special
 *            case: a(1) = 3
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Extended Euclid: modular inverse of a modulo m (gcd(a,m)=1). */
static uint64_t modinv_uint64(uint64_t a, uint64_t m)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)(a % m);
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = t - q * newt;
        t = newt;
        newt = tmp;
        tmp = r - q * newr;
        r = newr;
        newr = tmp;
    }
    if (t < 0) t += m;
    return (uint64_t)t;
}

/* Quick power for 64‑bit integers, exponent fits in 64 bits. */
static uint64_t pow_uint64(uint64_t base, uint64_t exp)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1U) res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;

    /* -----------------------------------------------------------------
       Step 1: factorise n.
       ----------------------------------------------------------------- */
    struct { uint64_t p; uint8_t e; } f[16];
    size_t nf = 0;
    uint64_t tmp = n;

    /* factor 2 */
    uint8_t e2 = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e2;
    }
    if (e2) {
        f[nf].p = 2;
        f[nf].e = e2;
        ++nf;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p) continue;
        uint8_t ee = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++ee;
        }
        f[nf].p = p;
        f[nf].e = ee;
        ++nf;
    }
    if (tmp > 1) {
        f[nf].p = tmp;
        f[nf].e = 1;
        ++nf;
    }

    /* -----------------------------------------------------------------
       Step 2: build for each prime power the modulus and residues
       ----------------------------------------------------------------- */
    struct { uint64_t mod; uint8_t rcount; uint64_t res[4]; } mr[16];
    for (size_t i = 0; i < nf; ++i) {
        uint64_t p = f[i].p;
        uint8_t e = f[i].e;
        uint64_t mod = pow_uint64(p, 2U * e);
        mr[i].mod = mod;
        if (p == 2U) {
            if (e == 1U) {                 /* modulus 4 */
                mr[i].rcount = 2;
                mr[i].res[0] = 1U;
                mr[i].res[1] = mod - 1U;
            } else {                       /* e >= 2, modulus >= 16 */
                mr[i].rcount = 4;
                mr[i].res[0] = 1U;
                mr[i].res[1] = mod - 1U;
                uint64_t half = 1ULL << (2U * e - 1U);   /* 2^(2e-1) */
                mr[i].res[2] = (1U + half) % mod;
                mr[i].res[3] = ((mod - 1U) + half) % mod;
            }
        } else {                           /* odd prime: ±1 only */
            mr[i].rcount = 2;
            mr[i].res[0] = 1U;
            mr[i].res[1] = mod - 1U;
        }
    }

    /* -----------------------------------------------------------------
       Step 3: combine residues via Chinese Remainder Theorem
       ----------------------------------------------------------------- */
    uint64_t cur_res[64] = {0};
    uint64_t cur_mod[64] = {1};
    size_t cur_len = 1;               /* start with modulus 1, residue 0 */

    for (size_t i = 0; i < nf; ++i) {
        uint64_t new_res[64];
        uint64_t new_mod[64];
        size_t new_len = 0;
        for (size_t j = 0; j < cur_len; ++j) {
            uint64_t a_rem = cur_res[j];
            uint64_t a_mod = cur_mod[j];
            for (uint8_t k = 0; k < mr[i].rcount; ++k) {
                uint64_t b_rem = mr[i].res[k];
                uint64_t b_mod = mr[i].mod;

                uint64_t a_rem_mod = a_rem % b_mod;
                uint64_t diff = (b_rem + b_mod - a_rem_mod) % b_mod;
                uint64_t inv = modinv_uint64(a_mod % b_mod, b_mod);
                uint64_t t = (unsigned __int128)diff * inv % b_mod;
                uint64_t x = a_rem + (unsigned __int128)a_mod * t;
                uint64_t new_modulus = (unsigned __int128)a_mod * b_mod;
                x %= new_modulus;

                new_res[new_len] = x;
                new_mod[new_len] = new_modulus;
                ++new_len;
            }
        }
        cur_len = new_len;
        for (size_t s = 0; s < cur_len; ++s) {
            cur_res[s] = new_res[s];
            cur_mod[s] = new_mod[s];
        }
    }

    /* -----------------------------------------------------------------
       Step 4: find the smallest m > 0
       ----------------------------------------------------------------- */
    uint64_t n_sq = n * n;
    uint64_t best_m = 0;
    bool first = true;
    for (size_t i = 0; i < cur_len; ++i) {
        uint64_t s = cur_res[i];
        if (s <= 1U) s += n_sq;        /* ensure s > 1 */
        uint64_t s_sq = s * s;
        uint64_t m = (s_sq - 1U) / n_sq;
        if (first || m < best_m) {
            best_m = m;
            first = false;
        }
    }
    return (seq_t)best_m;
}
