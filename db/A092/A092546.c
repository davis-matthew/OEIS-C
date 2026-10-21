/*
 * A092546  Molien series for complete weight enumerators of Type II self-dual
 *          codes over Z/8Z.
 * https://oeis.org/A092546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 28, u64 n <= 680, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A092546:
   Let N(x) = 1 + 4x +158x² +1134x³ +3964x⁴ +9015x⁵ +14318x⁶ +16531x⁷
                +14322x⁸ +9016x⁹ +3978x¹⁰ +1129x¹¹ +155x¹² +3x¹³
   Denominator D(x) = 1‑4x+5x²‑3x³+7x⁴‑11x⁵+2x⁶+3x⁷+3x⁸+2x⁹‑11x¹⁰+7x¹¹‑3x¹²+5x¹³‑4x¹⁴+x¹⁵
   If a(n) are the coefficients of N(x)/D(x) then for all n≥0
          a(n) = N_n – Σ_{k=1}^{min(n,15)} c_k · a(n‑k)
   where N_n is the coefficient of xⁿ in N(x) (0 for n>13) and
   c_k are the coefficients of D(x) for k=1…15. */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Numerator coefficients N(x), length 14 (degree 13). */
static const long long N_coef[14] = {
    1LL,   4LL, 158LL, 1134LL, 3964LL, 9015LL,
  14318LL,16531LL,14322LL, 9016LL, 3978LL,
   1129LL,  155LL,    3LL
};

/* Denominator coefficients c_k for k=1..15 from D(x). */
static const int C_coef[16] = {
    /* c0 is unused, set to 0 for simplicity */
    0,
   -4,   5,  -3,   7, -11,    2,
    3,    3,   2,  -11,    7,   -3,
    5,   -4,    1
};

#ifdef __cplusplus
static std::vector<seq_t> buf;
#else
static seq_t *buf = NULL;
static size_t buf_size = 0;
#endif

static void compute_up_to(idx_t n)
{
    size_t target = (size_t)n;
#ifdef __cplusplus
    if (buf.size() <= target) {
        size_t old = buf.size();
        buf.resize(target + 1);
        for (size_t i = old; i <= target; ++i) {
            seq_t sum = 0;
            if (i < 14) sum = (seq_t)N_coef[i];
            for (int k = 1; k <= 15 && k <= (int)i; ++k) {
                int ck = C_coef[k];
                if (ck < 0)
                    sum += buf[i - k] * (seq_t)(-ck);
                else
                    sum -= buf[i - k] * (seq_t)ck;
            }
            buf[i] = sum;
        }
    }
#else
    if (buf_size <= target) {
        size_t new_size = target + 1;
        seq_t *new_buf = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (!new_buf) return; /* allocation failed */
        buf = new_buf;
        for (size_t i = buf_size; i <= target; ++i) {
            seq_t sum = 0;
            if (i < 14) sum = (seq_t)N_coef[i];
            for (int k = 1; k <= 15 && k <= (int)i; ++k) {
                int ck = C_coef[k];
                if (ck < 0)
                    sum += buf[i - k] * (seq_t)(-ck);
                else
                    sum -= buf[i - k] * (seq_t)ck;
            }
            buf[i] = sum;
        }
        buf_size = new_size;
    }
#endif
}

seq_t a(idx_t n)
{
    compute_up_to(n);
#ifdef __cplusplus
    return buf[(size_t)n];
#else
    return buf[(size_t)n];
#endif
}
