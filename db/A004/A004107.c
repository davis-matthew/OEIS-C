/*
 * A004107  Number of self-dual nets with 2n nodes.
 * https://oeis.org/A004107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{partitions (m_1,...,m_k) of n} 2^k * 3^E / z, with E
 *            = Sum_i (m_i - 1 + [m_i even]) + Sum_{i<j} 2*gcd(m_i,m_j), z =
 *            Prod_i (2*m_i) * Prod_m (mult of m)!. This is de Bruijn's count
 *            of nets on 2n nodes that are fixed by sigma∘(sign swap), with
 *            sigma running over the permutations whose c...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   a4107_g, a4107_powmod, is_prime, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a4107_g.h"
#include "../utils/a4107_powmod.h"

#define A4107_MAXN 128
#define A4107_MAXE (A4107_MAXN * A4107_MAXN + 1)

static int a4107_parts[A4107_MAXN + 1];
static int a4107_gcd[A4107_MAXN + 1][A4107_MAXN + 1];
static seq_t a4107_bucket[A4107_MAXE];
static uint64_t a4107_bucketm[A4107_MAXE];
static int a4107_big;
static uint64_t a4107_p;
static seq_t a4107_fact;

static void a4107_leaf(int k, int E)
{
    int i, run = 0;
    if (a4107_big) {
        seq_t z = 1, t;
        for (i = 0; i < k; i++) {
            z *= (seq_t)(2 * a4107_parts[i]);
            if (i > 0 && a4107_parts[i] == a4107_parts[i - 1]) run++;
            else run = 1;
            z *= (seq_t)run;
        }
        t = a4107_fact / z;
        t = t << k;
        a4107_bucket[E] += t;
    } else {
        uint64_t p = a4107_p, z = 1, t;
        for (i = 0; i < k; i++) {
            z = z * (uint64_t)(2 * a4107_parts[i]) % p;
            if (i > 0 && a4107_parts[i] == a4107_parts[i - 1]) run++;
            else run = 1;
            z = z * (uint64_t)run % p;
        }
        t = a4107_powmod(z, p - 2, p) * a4107_powmod(2, (uint64_t)k, p) % p;
        a4107_bucketm[E] = (a4107_bucketm[E] + t) % p;
    }
}

static void a4107_dfs(int rem, int maxpart, int k, int E)
{
    int m, i;
    if (rem == 0) {
        a4107_leaf(k, E);
        return;
    }
    for (m = (rem < maxpart ? rem : maxpart); m >= 1; m--) {
        int add = (m - 1) + (m % 2 == 0 ? 1 : 0);
        for (i = 0; i < k; i++) add += 2 * a4107_gcd[m][a4107_parts[i]];
        a4107_parts[k] = m;
        a4107_dfs(rem - m, m, k + 1, E + add);
    }
}

seq_t a(idx_t n)
{
    int nn, i, j, Emax;
    if (n > A4107_MAXN) return 0;
    nn = (int)n;
    Emax = nn * nn;
    for (i = 0; i <= nn; i++)
        for (j = 0; j <= nn; j++) a4107_gcd[i][j] = a4107_g(i, j);

    if (seq_is_big()) {
        seq_t res = 0;
        int e;
        a4107_big = 1;
        a4107_fact = 1;
        for (i = 1; i <= 2 * nn; i++) a4107_fact *= (seq_t)i;
        for (e = 0; e <= Emax; e++) a4107_bucket[e] = 0;
        a4107_dfs(nn, nn, 0, 0);
        for (e = Emax; e >= 0; e--) res = res * 3 + a4107_bucket[e];
        return res / a4107_fact;
    } else {
        uint64_t primes[5], r[5];
        uint64_t cand = 2147483647ULL;
        int cnt = 0, q, e;
        unsigned __int128 x = 0, M = 1;
        while (cnt < 5) {
            if (is_prime(cand)) primes[cnt++] = cand;
            cand--;
        }
        a4107_big = 0;
        for (q = 0; q < 5; q++) {
            uint64_t p = primes[q], res = 0;
            a4107_p = p;
            for (e = 0; e <= Emax; e++) a4107_bucketm[e] = 0;
            a4107_dfs(nn, nn, 0, 0);
            for (e = Emax; e >= 0; e--) res = (res * 3 + a4107_bucketm[e]) % p;
            r[q] = res;
        }
        for (q = 0; q < 5; q++) {
            uint64_t p = primes[q];
            uint64_t xm = (uint64_t)(x % p);
            uint64_t Mm = (uint64_t)(M % p);
            uint64_t d = (r[q] + p - xm) % p * a4107_powmod(Mm, p - 2, p) % p;
            x += (unsigned __int128)d * M;
            M *= p;
        }
        return u128_to_seq(x);
    }
}
