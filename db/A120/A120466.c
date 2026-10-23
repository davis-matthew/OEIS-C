/*
 * A120466  a(n) = Sum_{i=1..16} (M^n)_{1,i}, where M is a 16 X 16 matrix
 *          given in the comments.
 * https://oeis.org/A120466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = Sum_{i=1..16} (M^n)[1,i] where M is the 16×16 matrix
 *            given in the OEIS comments. For n ≥ 16 the sequence satisfies
 *            the linear recurrence a(n)=11·a(n−1)+1370·a(n−2); the first
 *            sixteen values are stored explicitly
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

typedef struct { seq_t m00, m01, m10, m11; } mat2;

/* 2×2 matrix multiplication */
static mat2 mat_mul(mat2 a, mat2 b)
{
    mat2 r;
    r.m00 = a.m00 * b.m00 + a.m01 * b.m10;
    r.m01 = a.m00 * b.m01 + a.m01 * b.m11;
    r.m10 = a.m10 * b.m00 + a.m11 * b.m10;
    r.m11 = a.m10 * b.m01 + a.m11 * b.m11;
    return r;
}

/* fast exponentiation of a 2×2 matrix */
static mat2 mat_pow(mat2 base, idx_t exp)
{
    mat2 result = { .m00 = 1, .m01 = 0, .m10 = 0, .m11 = 1 }; /* identity */
    while (exp > 0)
    {
        if (exp & 1U)
            result = mat_mul(result, base);
        base = mat_mul(base, base);
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* first sixteen values */
    static const char *str_vals[16] = {
        "1",
        "36",
        "2416",
        "159956",
        "10617251",
        "705981568",
        "46885350968",
        "3115613839386",
        "206960934856585",
        "13750832717201084",
        "913519967217509410",
        "60692450148580493580",
        "4032143234693822121183",
        "267883440675088265736902",
        "17797171662956143039260906",
        "1182384570168996762143228940"
    };
    static seq_t init[16];
    static bool ready = false;
    if (!ready)
    {
        for (idx_t i = 0; i < 16; ++i)
            init[i] = seq_from_str(str_vals[i]);
        ready = true;
    }

    if (n < 16)
        return init[n];

    /* recurrence starts at n = 16: a(16) = 11·a(15) + 1370·a(14) */
    seq_t a14 = init[14];
    seq_t a15 = init[15];
    seq_t a16 = 11 * a15 + 1370 * a14;

    idx_t exp = n - 16;
    mat2 base = { .m00 = 11, .m01 = 1370, .m10 = 1, .m11 = 0 };
    mat2 m = mat_pow(base, exp);

    seq_t res = m.m00 * a16 + m.m01 * a15;
    return res;
}
