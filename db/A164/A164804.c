/*
 * A164804  a(n) = smallest number which has in its Spanish name the letter
 *          "u" in the n-th position, or -1 if no such number exists.
 * https://oeis.org/A164804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, sign
 * Formula:   a(n) = smallest integer whose Spanish word (without spaces and
 *            accents) contains a 'u' at position n, or -1 if none
 * Range:     i8 n <= 3, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   write_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/write_str.h"

static const char *UNIT_STR[20] = {"cero","uno","dos","tres","cuatro","cinco","seis","siete","ocho","nueve",
                                    "diez","once","doce","trece","catorce","quince","dieciseis",
                                    "diecisiete","dieciocho","diecinueve"};
static const char *TEN_STR[10]  = {"","", "veinte","treinta","cuarenta","cincuenta","sesenta","setenta",
                                   "ochenta","noventa"};
static const char *HUND_STR[10] = {"","ciento","doscientos","trescientos","cuatrocientos",
                                   "quinientos","seiscientos","setecientos","ochocientos","novecientos"};

static void write_rest(uint64_t n, char **p);
static void write_hundreds(uint64_t n, char **p);
static void write_number(uint64_t n, char **p);

static void write_rest(uint64_t n, char **p)
{
    if (n < 10) {
        write_str(UNIT_STR[n], p);
    } else if (n < 20) {
        write_str(UNIT_STR[n], p);
    } else if (n == 20) {
        write_str(TEN_STR[2], p);
    } else if (n < 30) {
        write_str("veinti", p);
        write_str(UNIT_STR[n - 20], p);
    } else {
        uint64_t t = n / 10;
        uint64_t u = n % 10;
        write_str(TEN_STR[t], p);
        if (u) {
            write_str("y", p);
            write_str(UNIT_STR[u], p);
        }
    }
}

static void write_hundreds(uint64_t n, char **p)
{
    uint64_t h = n / 100;
    uint64_t r = n % 100;
    if (h) {
        if (h == 1) {
            if (r == 0)
                write_str("cien", p);
            else
                write_str("ciento", p);
        } else {
            write_str(HUND_STR[h], p);
        }
    }
    if (r)
        write_rest(r, p);
}

static void write_number(uint64_t n, char **p)
{
    if (n == 0) {
        write_str("cero", p);
        return;
    }
    if (n >= 1000) {
        uint64_t thousands = n / 1000;
        uint64_t rest = n % 1000;
        if (thousands == 1)
            write_str("mil", p);
        else {
            write_number(thousands, p);
            write_str("mil", p);
        }
        if (rest)
            write_hundreds(rest, p);
    } else {
        write_hundreds(n, p);
    }
}

#define MAX_POS 10000
static seq_t MIN_POS[MAX_POS + 1];
static bool INIT_DONE = false;

seq_t a(idx_t n)
{
    if (!INIT_DONE) {
        for (int i = 0; i <= MAX_POS; ++i)
            MIN_POS[i] = (seq_t)-1;
        const uint64_t LIMIT_NUM = 2000000ULL;
        char buf[256];
        for (uint64_t num = 1; num <= LIMIT_NUM; ++num) {
            char *p = buf;
            write_number(num, &p);
            *p = 0;
            for (int i = 0; buf[i] != 0; ++i) {
                if (buf[i] == 'u' || buf[i] == 'U') {
                    int pos = i + 1;
                    if (pos <= MAX_POS && MIN_POS[pos] == (seq_t)-1)
                        MIN_POS[pos] = (seq_t)num;
                }
            }
        }
        INIT_DONE = true;
    }
    if (n <= 0 || n > MAX_POS)
        return (seq_t)-1;
    return MIN_POS[n];
}
