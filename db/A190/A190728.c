/*
 * A190728  Least m>1 such that m^3 mod n^2 is 1.
 * https://oeis.org/A190728
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>1 such that m^3 ≡ 1 (mod n^2)
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t modmul_uint64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t modpow3_uint64(uint64_t a, uint64_t m)
{
    uint64_t ab = modmul_uint64(a, a, m);
    return modmul_uint64(ab, a, m);
}

static uint64_t modinv_uint64(uint64_t a, uint64_t m)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)(a % m);
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = t - q * newt; t = newt; newt = tmp;
        tmp = r - q * newr; r = newr; newr = tmp;
    }
    if (r > 1) return 0;          /* not invertible, but this never happens here */
    if (t < 0) t += m;
    return (uint64_t)t;
}

/* Simple trial division factorization for n ≤ 10000 */
static void factorize(uint64_t n, uint64_t primes[10], int exps[10], int *cnt)
{
    int c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            primes[c] = p;
            exps[c] = 0;
            while (n % p == 0) {
                ++exps[c];
                n /= p;
            }
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* n ≥ 2 as per sequence definition */
    uint64_t các = (uint64_t)n;

    /* 1. Factor n */
    uint64_t primes[10];
    int exps[10];
    int fcnt;
    factorize(các, primes, exps, &fcnt);

    /* 2. Quick check for large trivial case */
    int cond = 0;
    for (int i = 0; i < fcnt; ++i) {
        if (các % 3 == 0 || (primes[i] % 3 == 1)) { cond = 1; break; }
    }
    seq_t nsq = (seq_t)các * (seq_t)các;
    if (!cond) return nsq + 1;          /* return n^2 + 1 */

    /* 3. Prepare per prime power solutions */
    const int MAX_FACT = 10;
    const int MAX_SOL  = 81;
    uint64_t mod_i[MAX_FACT];
    int sol_count[MAX_FACT];
    uint64_t sol_buf[MAX_FACT][3];
    int cur_factors = 0;

    for (int i = 0; i < fcnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        uint64_t mod_p2 = 1;
        for (int k = 0; k < 2 * e; ++k) mod_p2 *= p;
        mod_i[cur_factors] = mod_p2;

        if (p == 2) {
            sol_buf[cur_factors][0] = 1;
            sol_count[cur_factors] = 1;
        } else if (p == 3 || p % 3 == 1) {
            /* find initial cube roots modulo p */
            uint64_t init_sols[3];
            int init_cnt = 0;
            for (uint64_t m = 1; m < p; ++m)
                if (modpow3_uint64(m, p) == 1)
                    init_sols[init_cnt++] = m;

            uint64_t cur_sols[3];
            for (int j = 0; j < init_cnt; ++j) cur_sols[j] = init_sols[j];
            int cur_cnt = init_cnt;
            uint64_t mod_curr = p;
            while (mod_curr < mod_p2) {
                uint64_t mod_next = mod_curr * p;
                uint64_t new_sols[3];
                int new_cnt = 0;
                for (int j = 0; j < cur_cnt; ++j) {
                    uint64_t r = cur_sols[j];
                    for (uint64_t k = 0; k < p; ++k) {
                        uint64_t cand = r + k * mod_curr;
                        if (modpow3_uint64(cand, mod_next) == 1) {
                            /* deduplicate */
                            int dup = 0;
                            for (int t = 0; t < new_cnt; ++t)
                                if (new_sols[t] == cand) { dup = 1; break; }
                            if (!dup) new_sols[new_cnt++] = cand;
                        }
                    }
                }
                for (int t = 0; t < new_cnt; ++t) cur_sols[t] = new_sols[t];
                cur_cnt = new_cnt;
                mod_curr = mod_next;
            }
            for (int j = 0; j < cur_cnt; ++j) sol_buf[cur_factors][j] = cur_sols[j];
            sol_count[cur_factors] = cur_cnt;
        } else {
            sol_buf[cur_factors][0] = 1;
            sol_count[cur_factors] = 1;
        }
        ++cur_factors;
    }

    /* 4. Combine all prime powers via CRT */
    uint64_t sols[MAX_SOL];
    int sols_cnt = 0;
    for (int i = 0; i < sol_count[0]; ++i) sols[sols_cnt++] = sol_buf[0][i];
    uint64_t cur_mod = mod_i[0];

    for (int fi = 1; fi < cur_factors; ++fi) {
        uint64_t new_sols[MAX_SOL];
        int new_cnt = 0;
        uint64_t mod_j = mod_i[fi];
        uint64_t inv = modinv_uint64(cur_mod % mod_j, mod_j);
        for (int r = 0; r < sols_cnt; ++r) {
            uint64_t x = sols[r];
            for (int s = 0; s < sol_count[fi]; ++s) {
                uint64_t av = sol_buf[fi][s];
                uint64_t xm = x % mod_j;
                uint64_t diff = (av >= xm) ? (av - xm) : (mod_j - (xm - av));
                uint64_t k = (diff * inv) % mod_j;
                uint64_t nx = x + k * cur_mod;
                new_sols[new_cnt++] = nx;
            }
        }
        for (int t = 0; t < new_cnt; ++t) sols[t] = new_sols[t];
        sols_cnt = new_cnt;
        cur_mod *= mod_j;
    }

    /* 5. Find minimal solution > 1 */
    uint64_t best = (uint64_t)-1;
    for (int i = 0; i < sols_cnt; ++i) {
        uint64_t v = sols[i];
        if (v > 1 && v < best) best = v;
    }
    if (best == (uint64_t)-1) best = (uint64_t)nsq + 1; /* safety fallback */

    return (seq_t)best;
}
