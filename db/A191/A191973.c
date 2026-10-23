/*
 * A191973  Irregular triangle read by rows: row n consists of n and the
 *          positive integers m where m-n divides m*n.
 * https://oeis.org/A191973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is obtained by reading the rows of an irregular triangle.
 *            Row n consists of the integer n together with every integer m>0
 *            such that (m-n) divides m*n. Since (m-n)|(m*n) ⇔ (m-n)|n², we
 *            generate all (m-n)=d with d|n² (d≠0) and m=n+d. The linear
 *            sequence a(n) is the concatenation of all such rows
 * Range:     u8 n <= 151, u16 n <= 5996, u32 n <= 5996, u64 n <= 5996,
 *            u128 n <= 5996, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* === standard includes === */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* comparison for qsort on unsigned 64‑bit values */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* generate all positive divisors of n² into divs[0 .. *cnt-1].
   primes[0..k-1] are the prime factors of n,
   exp[0..k-1] are twice the exponents of those primes in n (i.e. exponents in n²). */
static void gen_divs(const uint64_t *primes, const int *exp, size_t idx, size_t k,
                     uint64_t cur, uint64_t *divs, size_t *cnt)
{
    if (idx == k) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t prod = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divs(primes, exp, idx + 1, k, prod, divs, cnt);
        prod = prod * primes[idx];
    }
}

/* ========================================================= */
seq_t a(idx_t n)
{
    /* static cache for already generated rows */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;   /* number of elements stored */
    static size_t seq_cap = 0;   /* allocated capacity */
    static idx_t next_row = 1;   /* next n to generate */

    if (n == 0) return 0;   /* safety: offset is 1 */

    while (seq_len < (size_t)n) {
        idx_t cur_n = next_row++;
        /* factor cur_n ------------------------------------- */
        uint64_t r = (uint64_t)cur_n;
        uint64_t primes[32];
        int exp[32];
        size_t pf_cnt = 0;

        for (uint64_t p = 2; p * p <= r; ++p) {
            if (r % p == 0) {
                int cnt = 0;
                while (r % p == 0) {
                    r /= p;
                    ++cnt;
                }
                primes[pf_cnt] = p;
                exp[pf_cnt] = cnt * 2;   /* exponent in n² */
                ++pf_cnt;
            }
        }
        if (r > 1) {           /* remaining prime factor */
            primes[pf_cnt] = r;
            exp[pf_cnt] = 2;     /* exponent 1 in n → 2 in n² */
            ++pf_cnt;
        }

        /* positive divisors of n² -------------------------------- */
        size_t div_cnt = 1;
        for (size_t i = 0; i < pf_cnt; ++i)
            div_cnt *= (size_t)(exp[i] + 1);

        uint64_t *divs = NULL;
        if (div_cnt > 0) {
            divs = (uint64_t *)malloc(div_cnt * sizeof(uint64_t));
            if (!divs) exit(EXIT_FAILURE);
            size_t dc = 0;
            gen_divs(primes, exp, 0, pf_cnt, (uint64_t)1, divs, &dc);
        }

        /* build row values ------------------------------------- */
        size_t row_cap = 2 * div_cnt + 1;   /* maximum possible size */
        uint64_t *row = (uint64_t *)malloc(row_cap * sizeof(uint64_t));
        if (!row) exit(EXIT_FAILURE);
        size_t rc = 0;

        uint64_t n_seq = (uint64_t)cur_n;
        row[rc++] = n_seq;   /* include n itself */

        for (size_t i = 0; i < div_cnt; ++i) {
            uint64_t d = divs[i];
            /* m = n + d (always positive) */
            row[rc++] = n_seq + d;
            /* m = n - d is valid only if d < n */
            if (d < n_seq) {
                row[rc++] = n_seq - d;
            }
        }

        /* sort the row by true integer value ------------------- */
        qsort(row, rc, sizeof(uint64_t), cmp_uint64);

        /* append sorted values to global sequence ------------ */
        if (seq_len + rc > seq_cap) {
            size_t new_cap = (seq_cap == 0) ? 128 : seq_cap * 2;
            while (new_cap < seq_len + rc) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            seq = tmp;
            seq_cap = new_cap;
        }
        for (size_t i = 0; i < rc; ++i)
            seq[seq_len++] = (seq_t)row[i];

        free(row);
        free(divs);
    }

    return seq[n - 1];
}
