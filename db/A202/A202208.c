/*
 * A202208  Smallest square (>4n) == 1 mod 4n.
 * https://oeis.org/A202208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest square > 4∙n such that k² ≡ 1 (mod 4∙n)
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root (floor) for 64‑bit values */
static uint64_t isqrt_uint64(uint64_t v) {
    uint64_t low = 0, high = v;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* modular inverse using extended Euclid, 0 if not invertible  */
static uint64_t modinv(uint64_t a, uint64_t mod) {
    __int128 t = 0, newt = 1;
    __int128 r = mod, newr = a % mod;
    while (newr != 0) {
        uint64_t q = (uint64_t)(r / newr);
        __int128 tmp = t - (__int128)q * newt;
        t = newt;
        newt = tmp;
        tmp = r - (__int128)q * newr;
        r = newr;
        newr = tmp;
    }
    if (r > 1) return 0;            /* not invertible, should not happen */
    if (t < 0) t += mod;
    return (uint64_t)t;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for 0, guard */
    uint64_t N = (uint64_t)n;
    uint64_t M = 4ULL * N;             /* modulus */

    /* factor 2-adic part */
    int t = 0;
    uint64_t tmp = M;
    while ((tmp & 1ULL) == 0) { t++; tmp >>= 1; }
    uint64_t mod2 = 1ULL << t;         /* 2^t */

    /* component struct for CRT */
    struct component {
        uint64_t mod;
        int      opt;
        uint64_t opt_res[4];
    } comps[64];
    int comp_cnt = 0;

    /* 2-part residues */
    comps[comp_cnt].mod = mod2;
    if (t == 2) {                          /* 4 */
        comps[comp_cnt].opt = 2;
        comps[comp_cnt].opt_res[0] = 1;
        comps[comp_cnt].opt_res[1] = mod2 - 1;   /* 3 */
    } else {                               /* 2^t with t>=3 */
        comps[comp_cnt].opt = 4;
        comps[comp_cnt].opt_res[0] = 1;
        comps[comp_cnt].opt_res[1] = (1ULL << (t-1)) + 1;  /* 1+2^{t-1} */
        comps[comp_cnt].opt_res[2] = mod2 - 1;              /* -1 */
        comps[comp_cnt].opt_res[3] = (1ULL << (t-1)) - 1;  /* -1+2^{t-1} */
    }
    comp_cnt++;

    /* factor odd part */
    uint64_t odd = tmp;                   /* tmp is odd after removing 2's */
    for (uint64_t p = 3; p * p <= odd; p += 2) {
        if (odd % p == 0) {
            uint64_t pw = 1;
            while (odd % p == 0) {
                pw *= p;
                odd /= p;
            }
            comps[comp_cnt].mod = pw;
            comps[comp_cnt].opt = 2;
            comps[comp_cnt].opt_res[0] = 1;
            comps[comp_cnt].opt_res[1] = pw - 1;
            comp_cnt++;
        }
    }
    if (odd > 1) {
        comps[comp_cnt].mod = odd;
        comps[comp_cnt].opt = 2;
        comps[comp_cnt].opt_res[0] = 1;
        comps[comp_cnt].opt_res[1] = odd - 1;
        comp_cnt++;
    }

    /* minimal k must exceed sqrt(4n) */
    uint64_t sqrt_floor = isqrt_uint64(M);
    uint64_t min_needed = sqrt_floor + 1;  /* > sqrt(4n) */

    uint64_t best_k = UINT64_MAX;
    int idx[64] = {0};

    /* enumerate all combinations of residues */
    bool finished = false;
    while (!finished) {
        /* combine via CRT */
        uint64_t r = 0, mod = 1;
        for (int c = 0; c < comp_cnt; ++c) {
            uint64_t mi = comps[c].mod;
            uint64_t ri = comps[c].opt_res[idx[c]];
            uint64_t inv = modinv(mod % mi, mi);
            uint64_t diff = (ri + mi - (r % mi)) % mi;
            uint64_t tval = (__uint128_t)diff * inv % mi;
            __uint128_t newr = (__uint128_t)mod * tval + r;
            __uint128_t newm = (__uint128_t)mod * mi;
            r = (uint64_t)(newr % newm);
            mod = (uint64_t)newm;          /* equals M after all factors */
        }

        /* choose smallest k ≥ min_needed with k ≡ r (mod M) */
        uint64_t cand_k;
        if (r >= min_needed) {
            cand_k = r;
        } else {
            uint64_t k_mult = (min_needed - r + M - 1) / M;
            cand_k = r + k_mult * M;
        }
        if (cand_k < best_k) best_k = cand_k;

        /* advance indices */
        for (int i = 0; i < comp_cnt; ++i) {
            idx[i]++;
            if (idx[i] == comps[i].opt) {
                idx[i] = 0;
            } else {
                break;
            }
            if (i == comp_cnt - 1) finished = true;
        }
    }

    seq_t result = (seq_t)best_k * (seq_t)best_k;
    return result;
}
