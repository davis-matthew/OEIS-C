/*
 * A007999  a(n) is the number of permutations w of 1,2,...,n such that both w
 *          and w^{-1} are alternating.
 * https://oeis.org/A007999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Sum_{k>=0} E_{2k+1}^2 u^(2k+1)/(2k+1)! + (1-x^2)^(-1/2)
 *            Sum_{k>=0} E_{2k}^2 u^(2k)/(2k)!, u = (1/2)*log((1+x)/(1-x)),
 *            E_j = A000111(j). With T(m,k) = m! [x^m] u^k/k! (T(0,0)=1,
 *            T(m+1,k) = T(m,k-1) + m(m-1) T(m-1,k)) and S(m) = Sum_k E_k^2
 *            T(m,k): n odd: n! a(n) = S(n); n even: n! a(n) = Sum...
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned long long u64;

static u64 mulm(u64 a, u64 b, u64 p) { return a * b % p; }

static u64 powm(u64 b, u64 e, u64 p) {
    u64 r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = mulm(r, b, p);
        b = mulm(b, b, p);
        e >>= 1;
    }
    return r;
}

static bool mr_prime(u64 n) {
    static const u64 bases[4] = {2, 3, 5, 7};
    u64 d, x;
    int s, i, r;
    if (n < 2) return false;
    for (i = 0; i < 4; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    d = n - 1; s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 4; i++) {
        x = powm(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = mulm(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

static u64 *prime_list = NULL;
static long prime_count = 0, prime_cap = 0;

static u64 get_prime(long i) {
    while (prime_count <= i) {
        u64 c;
        if (prime_count == prime_cap) {
            prime_cap = prime_cap ? 2 * prime_cap : 64;
            prime_list = (u64 *)realloc(prime_list, (size_t)prime_cap * sizeof(u64));
        }
        c = prime_count ? prime_list[prime_count - 1] - 2 : 2147483647ULL;
        while (!mr_prime(c)) c -= 2;
        prime_list[prime_count++] = c;
    }
    return prime_list[i];
}

/* a(N) mod p, p prime > N */
static u64 residue(long N, u64 p) {
    u64 *E2 = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *cur = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *prv = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *A = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *B = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *C = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *S = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *fact = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *ifact = (u64 *)calloc((size_t)N + 3, sizeof(u64));
    u64 *t, b, res;
    long m, k, j;

    /* Euler zigzag numbers via Seidel boustrophedon */
    prv[0] = 1;
    E2[0] = 1;
    for (m = 1; m <= N; m++) {
        cur[0] = 0;
        for (k = 1; k <= m; k++) cur[k] = (cur[k - 1] + prv[m - k]) % p;
        E2[m] = mulm(cur[m], cur[m], p);
        t = prv; prv = cur; cur = t;
    }

    /* T rows: A = row m-1, B = row m, C = row m+1 */
    B[0] = 1;
    for (m = 0; m <= N; m++) {
        u64 s = 0, f;
        for (k = m % 2; k <= m; k += 2) s = (s + mulm(E2[k], B[k], p)) % p;
        S[m] = s;
        if (m == N) break;
        f = (m == 0) ? 0 : mulm((u64)m, (u64)(m - 1), p);
        for (k = 0; k <= m + 1; k++)
            C[k] = ((k ? B[k - 1] : 0) + mulm(f, A[k], p)) % p;
        t = A; A = B; B = C; C = t;
    }

    fact[0] = 1;
    for (m = 1; m <= N; m++) fact[m] = mulm(fact[m - 1], (u64)m, p);
    ifact[N] = powm(fact[N], p - 2, p);
    for (m = N; m >= 1; m--) ifact[m - 1] = mulm(ifact[m], (u64)m, p);

    if (N % 2 == 1) {
        b = S[N];
    } else {
        u64 dd = 1;
        b = 0;
        for (j = 0; 2 * j <= N; j++) {
            u64 bin, term;
            if (j >= 1) dd = mulm(dd, (u64)(2 * j - 1), p);
            bin = mulm(fact[N], mulm(ifact[2 * j], ifact[N - 2 * j], p), p);
            term = mulm(mulm(bin, mulm(dd, dd, p), p), S[N - 2 * j], p);
            b = (b + term) % p;
        }
    }
    res = mulm(b, ifact[N], p);

    free(E2); free(cur); free(prv); free(A); free(B); free(C);
    free(S); free(fact); free(ifact);
    return res;
}

seq_t a(idx_t n) {
    long N = (long)n, k, i, j, r;
    long bits = 1;
    u64 *P, *D;
    seq_t v;

    /* upper bound on bit length of N! >= a(N) */
    for (k = 2; k <= N; k++) {
        long bl = 0;
        u64 x = (u64)k;
        while (x) { bl++; x >>= 1; }
        bits += bl;
    }
    r = bits / 30 + 1;   /* each prime > 2^30 */

    P = (u64 *)malloc((size_t)r * sizeof(u64));
    D = (u64 *)malloc((size_t)r * sizeof(u64));
    for (i = 0; i < r; i++) P[i] = get_prime(i);

    /* Garner mixed-radix digits */
    for (i = 0; i < r; i++) {
        u64 p = P[i];
        u64 ri = residue(N, p);
        u64 val = 0, prod = 1;
        for (j = 0; j < i; j++) {
            val = (val + mulm(D[j] % p, prod, p)) % p;
            prod = mulm(prod, P[j] % p, p);
        }
        D[i] = mulm((ri + p - val) % p, powm(prod, p - 2, p), p);
    }

    v = 0;
    for (i = r - 1; i >= 0; i--) {
        v = v * (seq_t)(idx_t)P[i] + (seq_t)(idx_t)D[i];
    }

    free(P);
    free(D);
    return v;
}
